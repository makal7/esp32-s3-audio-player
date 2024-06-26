#include "displayTools.hpp"

void DisplayTools::drawDisplayMenu(std::string title, std::vector<File_> list, int selected, U8G2_SH1106_128X64_NONAME_F_HW_I2C &display){
    int pages = 0;
    int pageA = 0;
    int pageSize = 0;

    if (list.size() > 0){
        pages = (list.size()-1) / 4;
    }else{
        pages = 0;
    }
    
    pageA = selected / 4;

    if (list.size() >= 4){
        pageSize = 4;
        if((list.size() - 4 * pageA) < 4){
            pageSize = list.size() - 4 * pageA;
        }
    }else{
        pageSize = list.size();
    }

    std::string pg = std::to_string(pageA + 1) + "/" + std::to_string(pages + 1);

    display.clearBuffer();
    display.setFont(FONT_BIG);
    display.drawUTF8(0,15,title.c_str());
    display.setFont(FONT_SMALL);
    display.drawUTF8(105,10,pg.c_str());
    for(int i = 4 * pageA, y = 0; i < 4 * pageA + pageSize; i++, y+=10){
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