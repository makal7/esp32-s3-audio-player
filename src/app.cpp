#include "app.hpp"

App::App() : display(U8G2_R0, U8X8_PIN_NONE), encoder(CLK, DT) {
}

void App::initAudio() {
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21); //max volume
}

void App::initSD() {
    //sd is connected using spi
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    if (!SD.begin(SD_CS))
    {
        display.drawUTF8(0,11+11+4,"SD card init failed!");
        display.drawUTF8(0,11+11+11+4,"Please inser SD card ");
        display.drawUTF8(0,11+11+11+11+4,"and reboot!");
        display.sendBuffer();
        while(1);
    }
}

void App::initWifi() {
    //nvs flash is used by wifi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) 
    {
        display.clearBuffer();
        display.drawUTF8(0,11,"Connecting to wifi");
        delay(500);
        display.sendBuffer();
        delay(500);
    }
}

void App::init() { 
    displayTools.init(display);
    initAudio();
    initSD();
    #if WIFI
    initWifi();
    createRadioList();
    #endif

    //init buttons
    pinMode(SW, INPUT_PULLUP);
    pinMode(BACK_SW, INPUT_PULLDOWN);
}

void App::run(){
    init();

    while (1){
        selectMode();
        switch (mode)
        {
            case 0: mp3Player(); break;
            case 1: onlineRadio(); break;
            default: break;
        }
    }
}

void App::selectMode(){
    std::vector<File_> modes;
    File_ mode;

    mode.name = "mp3 player";
    modes.push_back(mode);
    #if WIFI
    mode.name = "online radio";
    modes.push_back(mode);
    #endif

    while(1){
        encoder.selectOption(this->mode, modes.size()-1);
        displayTools.drawDisplayMenu("Select Mode",modes,this->mode,display);

        if (digitalRead(SW) == LOW)
            return;
    }
}

void App::mp3Player(){
    vTaskDelay(250/portTICK_PERIOD_MS);
    createDirList("/");
    while(1){
        if(audio.getAudioCurrentTime() >= audio.getAudioFileDuration())
        {
            while(1){
                selectedSong = 0;
                selectSong();
                if (digitalRead(BACK_SW) == HIGH)
                    break;
                
                delay(250);

                //if selected file/directory is file then extit this loop 
                if (fileList.at(selectedSong).isDirectory == false)
                    break;

                if (fileList.at(selectedSong).isDirectory)
                    createDirList(fileList.at(selectedSong).path);
            }

            if (digitalRead(BACK_SW) == HIGH){
                break;
            }

            audio.connecttoFS(SD, fileList.at(selectedSong).path.c_str());
            delay(1000);
        }

        displayTools.drawPlayerMenu("Playing:", fileList, selectedSong, display, audio);
        
        if (digitalRead(BACK_SW) == HIGH)
            audio.setAudioPlayPosition(audio.getAudioFileDuration() + 2);
    }
}

void App::selectSong(){
    while (1){
        encoder.selectOption(selectedSong, fileList.size()-1);
        displayTools.drawDisplayMenu("File Explorer", fileList, selectedSong, display);

        if (digitalRead(SW) == LOW || digitalRead(BACK_SW) == HIGH)
            return;
    } 
}

void App::createDirList(std::string dir){
    File root = SD.open(dir.c_str());
    File file = root.openNextFile();
    File_ f;
    fileList.clear();
 
    while(file){
        if(!file.isDirectory()){
            f.name = file.name();
            f.path = file.path();
            f.isDirectory = false;
            fileList.push_back(f);
        }
        else{
            f.name = "DIR: ";
            f.name += file.name();
            f.path = file.path();
            f.isDirectory = true;
            fileList.push_back(f);
        }
 
      file = root.openNextFile();
  }
}

void App::onlineRadio(){
    delay(250);
    audio.setConnectionTimeout(2000,2000);
    while(1){
        selectRadio();
        if (digitalRead(BACK_SW) == HIGH){
            break;
        }

        audio.connecttohost(radioList.at(selectedRadio).path.c_str());
        displayTools.drawPlayerMenu("Online Radio", radioList, selectedRadio, display, audio);
        while(1){
            if (digitalRead(BACK_SW) == HIGH){
                audio.stopSong();
                break;
            }
            delay(1);
        }
        delay(500);

    }
}

Audio* App::getAudio(){

    return &audio;
}

void App::selectRadio(){
    while (1){
        encoder.selectOption(selectedRadio, radioList.size()-1);
        displayTools.drawDisplayMenu("Radio Explorer", radioList, selectedRadio, display);
        if (digitalRead(SW) == LOW || digitalRead(BACK_SW) == HIGH)
            return;
  }
}

void App::createRadioList() {
    radioList.clear();

    File_ radio;

    radio.name = "Fun Rádio";
    radio.path = "http://stream.funradio.sk:8000/fun192.mp3";
    radioList.push_back(radio);

    radio.name = "Rádio Express";
    radio.path = "http://stream.bauermedia.sk/128.mp3?aw_0_req.gdpr=false&aw_0_1st.playerid=expres_web_desktop";
    radioList.push_back(radio);
}