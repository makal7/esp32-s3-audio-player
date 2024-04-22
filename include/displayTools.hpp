#pragma once

#include "file_.hpp"
#include <vector>
#include <U8g2lib.h>
#include "config.hpp"
#include <Audio.h>

class DisplayTools {
    public:
        void drawDisplayMenu(std::string title, std::vector<File_> list, int selected,U8G2_SH1106_128X64_NONAME_F_HW_I2C &display);
        void drawPlayerMenu(std::string title, std::vector<File_> list, int selected,U8G2_SH1106_128X64_NONAME_F_HW_I2C &display, Audio &audio);
        void init(U8G2_SH1106_128X64_NONAME_F_HW_I2C &display);
};