#pragma once

#include <Audio.h>
#include <U8g2lib.h>
#include <vector>
#include <nvs_flash.h>
#include "SD.h"
#include "FS.h"
#include <string>

#include "config.hpp"
#include "file_.hpp"
#include "encoder.hpp"
#include "displayTools.hpp"

class App
{
    public:
        App();
        void run();
        Audio* getAudio();

    
    private:
        void init();
        void initAudio();
        void initSD();
        void initWifi();
        void selectMode();
        void mp3Player();
        void onlineRadio();
        void selectSong();
        void selectRadio();
        void createRadioList();
        void createDirList(std::string dir);

        Audio audio;
        U8G2_SH1106_128X64_NONAME_F_HW_I2C display;
        Encoder encoder;
        DisplayTools displayTools;

        std::vector<File_> radioList;
        std::vector<File_> fileList;
        int mode;
        int selectedSong;
        int selectedRadio;

};