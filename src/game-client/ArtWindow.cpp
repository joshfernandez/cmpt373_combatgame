#include <ascii-converter/AsciiConverter.hpp>
#include <chrono>
#include <thread>
#include <glog/logging.h>
#include "ArtWindow.hpp"

gui::ArtWindow::ArtWindow() {
    mainWindow = newwin(LINES, COLS, 0, 0);
    CHECK(mainWindow) << "Error creating main window";

    //TODO center
    frameWindow = derwin(mainWindow, LINES, COLS - 20, 0, 20);
    CHECK(frameWindow) << "Error creating frame window";
}

void gui::ArtWindow::update(int input) {

}

void gui::ArtWindow::redraw() {

}

void gui::ArtWindow::resize(const gui::Size& maxSize) {

}

WINDOW* gui::ArtWindow::getCursesWindow() {
    return mainWindow;
}

void gui::ArtWindow::playSplash(const std::string& name) {
    std::vector<std::string> animatedsplash = AsciiConverter::animateSequence(name);
    playAnimation(animatedsplash, 120);
}

void gui::ArtWindow::showAnimation(const std::string& name) {
    auto anim = AsciiConverter::animateObject(name);
    playAnimation(anim, 120);
}

void gui::ArtWindow::playAnimation(const std::vector<std::string>& frames, int frameTimeMs) {
    for (const auto& frame : frames){
        wclear(frameWindow);
        auto cstr = frame.c_str();
        mvwprintw(frameWindow, 0, 0, cstr);
        wrefresh(frameWindow);
        this_thread::sleep_for(chrono::milliseconds(frameTimeMs)); //120 seems to work fine
    }
       this_thread::sleep_for(chrono::milliseconds(550));
}
