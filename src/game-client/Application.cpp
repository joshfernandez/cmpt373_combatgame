#include "Application.hpp"
#include <glog/logging.h>

namespace gui {

Application::Application() {
    initscr();
    noecho();
    cbreak();
    halfdelay(1);
    start_color();
    set_escdelay(25);

    clear();
    refresh();
    getmaxyx(stdscr, h, w);
}

void Application::switchToWindow(const std::string& name) {
    CHECK(windows.count(name) == 1) << "No window with name: " << name;

    if (name == currentWindowName) {
        return;
    }

    clear();
    refresh();

    if (currentWindow) {
        currentWindow->onExit();
    }

    currentWindow = windows.at(name);
    currentWindow->onEnter();
    currentWindow->redraw();

    currentWindowName = name;
}

void Application::update() {
    CHECK(currentWindow != nullptr) << "No window selected";

    if (is_term_resized(h, w)) {
        getmaxyx(stdscr, h, w);
        clear();
        resizeterm(h, w);
        refresh();
        currentWindow->resize(Size{w, h});
    }

    int input = wgetch(currentWindow->getCursesWindow());
    switch (input) {
        case ERR:
            break;
        default:
            currentWindow->update(input);
            break;
    }

    currentWindow->redraw();
}

Application::~Application() {
    endwin();
}

void Application::addWindow(const std::string& name, Window *window) {
    windows.insert({name, window});
    if (currentWindow == nullptr) {
        switchToWindow(name);
    }
}

const std::string& Application::getCurrentWindowName() {
    return currentWindowName;
}

}