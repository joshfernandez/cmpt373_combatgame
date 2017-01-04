#include "ChatWindow.hpp"
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>
#include "StrColorizer.hpp"

namespace {
const int ENTRY_WINDOW_HEIGHT = 3;
const int ENTRY_SUB_WINDOW_HEIGHT = 1;
}

namespace gui {

ChatWindow::ChatWindow() {
    displayWindow = newwin(0, 0, 0, 0);
    CHECK(displayWindow) << "Error creating display window";
    scrollok(displayWindow, true);

    entryWindow = newwin(0, 0, 0, 0);
    CHECK(entryWindow) << "Error creating entry window";
    keypad(entryWindow, true);
    entrySubWindow = derwin(entryWindow, 0, 0, 1, 1);
    CHECK(entrySubWindow) << "Error creating entry sub window";

    entryField = new_field(ENTRY_SUB_WINDOW_HEIGHT, COLS - 2, 0, 0, 0, 0);
    CHECK(entryField) << "Error creating entry field";
    set_field_opts(entryField, O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    field_opts_off(entryField, O_AUTOSKIP | O_WRAP);
    entryFields[0] = entryField;

    entryForm = new_form(entryFields);
    set_form_win(entryForm, entryWindow);
    set_form_sub(entryForm, entrySubWindow);
    post_form(entryForm);

    resize(Size{COLS, LINES});
}

ChatWindow::~ChatWindow() {
    unpost_form(entryForm);
    free_form(entryForm);
    free_field(entryField);
    delwin(displayWindow);
}

void ChatWindow::update(int input) {
    switch(input) {
        case '\n':
        case KEY_ENTER: {
            form_driver(entryForm, REQ_VALIDATION);
            auto text = getEntryText();
            if (text.empty()) {
                break;
            }

            if (callback) {
                callback(text);
            }

            historyIndex = -1;
            commandHistory.push_front(text);
            clearEntryText();
            break;
        }
        case KEY_BACKSPACE:
        case 127:
            form_driver(entryForm, REQ_DEL_PREV);
            break;
        case KEY_DC:
            form_driver(entryForm, REQ_DEL_CHAR);
            break;
        case KEY_LEFT:
            form_driver(entryForm, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(entryForm, REQ_NEXT_CHAR);
            break;
        case KEY_UP: {
            if(commandHistory.size() == 0) {
                break;
            } else if(historyIndex == commandHistory.size() - 1) {
                break;
            }

            auto text = commandHistory[++historyIndex];
            set_field_buffer(entryField, 0, text.c_str());
            form_driver(entryForm, REQ_END_FIELD);
            break;
        }
        case KEY_DOWN: {
            if (historyIndex <= 0) {
                clearEntryText();
                historyIndex = -1;
                break;
            }

            auto text = commandHistory[--historyIndex];
            set_field_buffer(entryField, 0, text.c_str());
            form_driver(entryForm, REQ_END_FIELD);
            break;
        }
        case 27:
            clearEntryText();
            break;
        case KEY_F(1):
            onSoftKeyPressed(SoftKey::F1);
            break;
        case KEY_F(2):
            onSoftKeyPressed(SoftKey::F2);
            break;
        default:
            form_driver(entryForm, input);
            break;
    }
}

void ChatWindow::redraw() {
    wrefresh(displayWindow);
    box(entryWindow, 0, 0);
    wrefresh(entryWindow);
}

void ChatWindow::resize(const Size& maxSize) {
    wclear(displayWindow);
    wclear(entryWindow);
    unpost_form(entryForm);

    wresize(displayWindow, maxSize.height - ENTRY_WINDOW_HEIGHT, maxSize.width);
    wresize(entryWindow, ENTRY_WINDOW_HEIGHT, maxSize.width);
    wresize(entrySubWindow, ENTRY_SUB_WINDOW_HEIGHT, maxSize.width - 2);

    mvwin(entryWindow, maxSize.height - ENTRY_WINDOW_HEIGHT, 0);
    move_field(entryField, maxSize.height - ENTRY_WINDOW_HEIGHT + 1, 0);

    reprintText();

    post_form(entryForm);
    pos_form_cursor(entryForm);
    set_field_buffer(entryField, 0, "");
    form_driver(entryForm, REQ_END_FIELD);
}

WINDOW* ChatWindow::getCursesWindow() {
    return entryWindow;
}

std::string ChatWindow::getEntryText() {
    auto entryText = std::string{field_buffer(entryField, 0), (size_t)COLS - 2};
    boost::trim(entryText);
    return entryText;
}

void ChatWindow::clearEntryText() {
    set_field_buffer(entryField, 0, "");
    pos_form_cursor(entryForm);
}

void ChatWindow::setOnInput(std::function<void(const std::string& text)> callback) {
    this->callback = callback;
}

void ChatWindow::onEnter() {
    Window::onEnter();
    curs_set(true);
    pos_form_cursor(entryForm);

    reprintText();
}

void ChatWindow::showText(const std::string& text) {
    printColorText(text);
    lineHistory.push_back(text);
}

void ChatWindow::setOnSoftKeyPressed(std::function<void(SoftKey)> callback) {
    softkeyPressed = callback;
}

void ChatWindow::onSoftKeyPressed(SoftKey key) {
    if (softkeyPressed) {
        softkeyPressed(key);
    }
}

void ChatWindow::onExit() {
    Window::onExit();
    wmove(displayWindow, 0, 0);
}

void ChatWindow::printColorText(const std::string& text) {
    StrColorizer::print_color(displayWindow, 0, 0, text + "\n\n");
}

void ChatWindow::reprintText() {
    int w;
    int lines;
    getmaxyx(displayWindow, lines, w);
    if (lineHistory.size() < lines ) {
        lines = (int)lineHistory.size(); //Make sure buffer size is less than max int
    }

    for (int i = lineHistory.size() - lines; i < lineHistory.size(); i++) {
        printColorText(lineHistory[i]);
    }
}

}