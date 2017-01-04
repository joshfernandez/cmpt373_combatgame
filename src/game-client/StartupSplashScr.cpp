//
//
//

#include "StartupSplashScr.hpp"


void StartupSplashScr::animateSplash() {

    WINDOW *win;
    struct winsize wind;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wind);

    //int beg_y = wind.ws_row /8;
     int beg_x = (wind.ws_col /2)- 45;

    win = newwin(wind.ws_row,wind.ws_col,0,beg_x);

    //initscr();

    std::vector<std::string> animatedsplash = AsciiConverter::animateSequence("splash");

    start_color();
    init_pair(1,COLOR_WHITE, COLOR_BLACK);
    wattron(win,COLOR_PAIR(1));

    for (string s: animatedsplash){

        const char *cr = s.c_str();
        print_middle(win,0,0,80,0,cr); //width height placeholders for now
        this_thread::sleep_for(chrono::milliseconds(120)); //120 seems to work fine
        wrefresh(win);

    }

    wattroff(win,COLOR_PAIR(1));
    wgetch(win);
    this_thread::sleep_for(chrono::milliseconds(500));


}

void StartupSplashScr::print_middle(WINDOW *win, int starty, int startx, int width, int height, const char* str) {

    //Modify function later to best fit printing needs

    mvwprintw(win,starty,startx,"%s", str);
    //refresh();


}