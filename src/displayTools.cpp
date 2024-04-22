#include "displayTools.hpp"

void DisplayTools::drawDisplayMenu(std::string title, std::vector<File_> list, int selected, U8G2_SH1106_128X64_NONAME_F_HW_I2C &display){
    int offset = 0;
    if (selected >= 3){
        offset = list.size() - (list.size() - selected);
    }

    display.clearBuffer();
    display.setFont(FONT_BIG);
    display.drawUTF8(0,15,title.c_str());
    display.setFont(FONT_SMALL);
    for(int i = 0 + offset, y = 0; i < list.size(); i++, y+=11){
        display.drawUTF8(2, 15+11+4+y, list.at(i).name.c_str());
        if(i == selected)	
            display.drawFrame(0, 15+4+y+1, 128, 12);
    }
    display.sendBuffer();
}

void DisplayTools::drawPlayerMenu(std::string title, std::vector<File_> list, int selected,U8G2_SH1106_128X64_NONAME_F_HW_I2C &display, Audio &audio){
    display.clearBuffer();
    display.setFont(FONT_BIG);
    display.drawUTF8(0,15,title.c_str());
    display.setFont(FONT_SMALL);
    display.drawUTF8(0, 15+11+4, list.at(selected).name.c_str());	
    char buffer[32];
    sprintf(buffer,"%d / %d",audio.getAudioCurrentTime(),audio.getAudioFileDuration());
    display.drawUTF8(0,15+11+11+4,buffer);
    display.sendBuffer();
}

void DisplayTools::init(U8G2_SH1106_128X64_NONAME_F_HW_I2C &display){
    display.begin();
    display.setFont(FONT_SMALL);
    display.drawUTF8(0,11,"Booting ...");
    display.sendBuffer();
}