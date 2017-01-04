//
//
//

#ifndef ADVENTURE2016_CLIENTSTARTUP_HPP
#define ADVENTURE2016_CLIENTSTARTUP_HPP

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <ncurses.h>
#include "ascii-converter/AsciiConverter.hpp"
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/ioctl.h>



class StartupSplashScr {
private:
    static void print_middle(WINDOW *win, int starty, int startx, int width, int height, const char* str);

public:

    //Uses default "splash" assets
    static void animateSplash();

    //Add if needed
    //static void animateSplash(std::string& name);

};


#endif //ADVENTURE2016_CLIENTSTARTUP_HPP
