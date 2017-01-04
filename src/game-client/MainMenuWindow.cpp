#include <menu.h>
#include "MainMenuWindow.hpp"
#include <unordered_map>
#include <glog/logging.h>

namespace {

const std::string TITLE = "ADVENTURE 2016";
const int SUB_MENU_WIDTH = TITLE.length();
const int MENU_NUM_ITEMS = 3;
const int MENU_WIDTH = SUB_MENU_WIDTH + 6;
const int MENU_HEIGHT = MENU_NUM_ITEMS + 4 ;
const int SUB_MENU_OFFSET_X = (MENU_WIDTH - 8) / 2;
const int SUB_MENU_OFFSET_Y = 2;
const int TITLE_OFFSET_X = (MENU_WIDTH - TITLE.length()) / 2;

const std::string LOGIN_CHOICE = "Login";
const std::string REGISTER_CHOICE = "Register";
const std::string EXIT_CHOICE = "Exit";

const std::string MENU_ITEM_NAMES[MENU_NUM_ITEMS] = {
        LOGIN_CHOICE,
        REGISTER_CHOICE,
        EXIT_CHOICE,
};

gui::MainMenuChoice getChoice(const std::string& val) {
    if (val == LOGIN_CHOICE) {
        return gui::MainMenuChoice::LOGIN;
    } else if (val == REGISTER_CHOICE) {
        return gui::MainMenuChoice::REGISTER;
    } else {
        return gui::MainMenuChoice::EXIT;
    }
}

}

namespace gui {

MainMenuWindow::MainMenuWindow() {
    int w = 0;
    int h = 0;
    getmaxyx(stdscr, h, w);
    size = Size{w, h};

    menuWindow = newwin(MENU_HEIGHT, MENU_WIDTH, 0, 0);
    CHECK(menuWindow) << "Error creating window";
    keypad(menuWindow, true);

    menuSubWindow = derwin(menuWindow, MENU_NUM_ITEMS, SUB_MENU_WIDTH, SUB_MENU_OFFSET_Y, SUB_MENU_OFFSET_X);
    CHECK(menuSubWindow) << "Error creating subwindow";

    createMenu();

    resize(Size{w, h});
}

void MainMenuWindow::update(int input) {
    switch(input) {
        case KEY_UP:
            menu_driver(menu, REQ_UP_ITEM);
            break;
        case KEY_DOWN:
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case '\n':
        case KEY_ENTER: {
            auto currentItem = current_item(menu);
            auto val = std::string{item_name(currentItem)};
            auto choice = getChoice(val);
            if (callback) {
                callback(choice);
            }
            break;
        }
        default:
            break;
    }
}

WINDOW* MainMenuWindow::getCursesWindow() {
    return menuWindow;
}

MainMenuWindow::~MainMenuWindow() {
    removeMenu();
    delwin(menuSubWindow);
    delwin(menuWindow);
}

void MainMenuWindow::redraw() {
    box(menuWindow, 0, 0);
    mvwprintw(menuWindow, 0, TITLE_OFFSET_X, TITLE.c_str());
    wrefresh(menuWindow);
}

void MainMenuWindow::resize(const Size& size) {
    this->size = size;
    recreate();
}

void MainMenuWindow::createMenu() {
    for (const auto& name : MENU_ITEM_NAMES) {
        auto itemName = name.c_str();
        menuItems.emplace_back(new_item(itemName, nullptr));
    }
    menuItems.emplace_back(nullptr);

    menu = new_menu(menuItems.data());
    set_menu_win(menu, menuWindow);
    set_menu_sub(menu, menuSubWindow);
    set_menu_mark(menu, "");

    post_menu(menu);
}

void MainMenuWindow::removeMenu() {
    unpost_menu(menu);
    free_menu(menu);

    for (ITEM* item : menuItems) {
        free_item(item);
    }
    menuItems.clear();
}

void MainMenuWindow::recreate() {
    int menuX = (size.width - MENU_WIDTH) / 2 ;
    int menuY = (size.height - MENU_HEIGHT) / 2 ;
    mvwin(menuWindow, menuY, menuX);

    set_menu_win(menu, menuWindow);
    set_menu_sub(menu, menuSubWindow);
}

void MainMenuWindow::setOnSelection(std::function<void(MainMenuChoice)> callback) {
    this->callback = callback;
}

void MainMenuWindow::onEnter() {
    Window::onEnter();
    curs_set(false);
}

}