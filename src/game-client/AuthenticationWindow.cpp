#include "AuthenticationWindow.hpp"
#include <string>
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>

namespace {
const int ENTRY_WIDTH = 30;
const int ENTRY_HEIGHT = 14;
const int WINDOW_WIDTH = ENTRY_WIDTH + 6; //padding
const int WINDOW_HEIGHT = ENTRY_HEIGHT + 6;
const int ENTRY_OFFSET_Y = 1;
const int ENTRY_OFFSET_X = (WINDOW_WIDTH - ENTRY_WIDTH) / 2;
const int FIELD_OFFSET_X = 0;
const int FIELD_WIDTH = ENTRY_WIDTH - FIELD_OFFSET_X * 2; // need space after offsetting
const int TITLE_OFFSET_Y = ENTRY_OFFSET_Y;
const int USERNAME_LABEL_OFFSET_Y = ENTRY_OFFSET_Y + 1;
const int USERNAME_INPUT_OFFSET_Y = ENTRY_OFFSET_Y + 2;
const int PASSWORD_LABEL_OFFSET_Y = ENTRY_OFFSET_Y + 3;
const int PASSWORD_INPUT_OFFSET_Y = ENTRY_OFFSET_Y + 4;
const int MESSAGE_FIELD_OFFSET_Y = ENTRY_OFFSET_Y + 7;
const int LOADING_FIELD_OFFSET_Y = ENTRY_OFFSET_Y + 9;

int loadingSpinnerIndex = 0;
const int NUM_SPINNER_STATES = 4;
std::string loadingSpinnerStates[NUM_SPINNER_STATES] = {"-", "\\", "|", "/"};

std::string passwordFill{"********************************"}; //Fill constructor doesnt work for some reason?

FIELD* createTitleField(const std::string& title) {
    FIELD* usernameLabelField = new_field(1, FIELD_WIDTH, TITLE_OFFSET_Y, FIELD_OFFSET_X, 0, 0);
    CHECK(usernameLabelField) << "Error creating title label field";
    set_field_buffer(usernameLabelField, 0, title.c_str());
    set_field_just(usernameLabelField, JUSTIFY_CENTER);
    set_field_opts(usernameLabelField, O_STATIC | O_VISIBLE | O_PUBLIC);
    set_field_type(usernameLabelField, TYPE_ALPHA);
    set_field_fore(usernameLabelField, A_BOLD);
    field_opts_off(usernameLabelField, O_AUTOSKIP | O_EDIT | O_ACTIVE);

    return usernameLabelField;
}

FIELD* createLabelField(const std::string label, int position) {
    FIELD* usernameLabelField = new_field(1, FIELD_WIDTH, position, FIELD_OFFSET_X, 0, 0);
    CHECK(usernameLabelField) << "Error creating  label field";
    set_field_buffer(usernameLabelField, 0, label.c_str());
    set_field_opts(usernameLabelField, O_STATIC | O_VISIBLE | O_PUBLIC);
    set_field_type(usernameLabelField, TYPE_ALPHA);
    field_opts_off(usernameLabelField, O_AUTOSKIP | O_EDIT | O_ACTIVE);

    return usernameLabelField;
}


FIELD* createInputField(int position) {
    FIELD* usernameField = new_field(1, FIELD_WIDTH, position, FIELD_OFFSET_X, 0, 0);
    CHECK(usernameField) << "Error creating input field";
    set_field_buffer(usernameField,0, "");
    set_field_opts(usernameField, O_STATIC | O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_back(usernameField, A_UNDERLINE);
    field_opts_off(usernameField, O_AUTOSKIP);

    return usernameField;
}

FIELD* createMessageField() {
    FIELD* messageField = new_field(2, FIELD_WIDTH, MESSAGE_FIELD_OFFSET_Y, FIELD_OFFSET_X, 0, 0);
    CHECK(messageField) << "Error creating message field";
    set_field_buffer(messageField, 0, "");
    set_field_opts(messageField, O_STATIC | O_VISIBLE | O_PUBLIC | O_WRAP | O_EDIT);
    field_opts_off(messageField, O_AUTOSKIP | O_ACTIVE);

    return messageField;
}

FIELD* createLoadingField() {
    FIELD* loadingField = new_field(1, FIELD_WIDTH, LOADING_FIELD_OFFSET_Y, FIELD_OFFSET_X, 0, 0);
    CHECK(loadingField) << "Error creating loading field";
    set_field_just(loadingField, JUSTIFY_CENTER);
    set_field_opts(loadingField, O_STATIC | O_VISIBLE | O_PUBLIC);
    set_field_fore(loadingField, A_BOLD);
    field_opts_off(loadingField, O_AUTOSKIP | O_EDIT | O_ACTIVE);

    return loadingField;
}

}

namespace gui {

AuthenticationWindow::AuthenticationWindow(std::string title) : title{std::move(title)} {
    entryWindow = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    CHECK(entryWindow) << "Error creating window";
    keypad(entryWindow, true);

    entrySubWindow = derwin(entryWindow, ENTRY_HEIGHT, ENTRY_WIDTH, 1, ENTRY_OFFSET_X);
    CHECK(entrySubWindow) << "Error creating sub window";

    createForm();
    resize(Size{COLS, LINES});
}

void AuthenticationWindow::update(int input) {
    switch(input) {
        case '\t':
        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case '\n':
        case KEY_ENTER:
            form_driver(form, REQ_VALIDATION);
            form_driver(form, REQ_END_LINE);

            processCredentials();
            pos_form_cursor(form);
            break;
        case 27:
            if (onCancelCallback) {
                onCancelCallback();
            }
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            form_driver(form, REQ_END_LINE);
            if (current_field(form) == passwordInputField) {
                updatePasswordField(false);
            }
            break;
        default:
            form_driver(form, input);
            if (current_field(form) == passwordInputField) {
                updatePasswordField(true);
            }
            break;
    }
}

void AuthenticationWindow::resize(const gui::Size& maxSize) {
    int x = (maxSize.width - WINDOW_WIDTH) / 2;
    int y = (maxSize.height - WINDOW_HEIGHT) / 2;
    mvwin(entryWindow, y, x);

    set_form_win(form, entryWindow);
    set_form_sub(form, entrySubWindow);
}

WINDOW* AuthenticationWindow::getCursesWindow() {
    return entryWindow;
}

AuthenticationWindow::~AuthenticationWindow() {
    unpost_form(form);
    free_form(form);
    for(const auto& field : fields) {
        free_field(field);
    }
}

void AuthenticationWindow::redraw() {
    box(entryWindow, 0, 0);

    if(isLoading) {
        loadingSpinnerIndex++;
        loadingSpinnerIndex %= NUM_SPINNER_STATES;
        set_field_buffer(loadingField, 0, loadingSpinnerStates[loadingSpinnerIndex].c_str());
    }

    wrefresh(entryWindow);
}

void AuthenticationWindow::createForm() {
    fields.push_back(createTitleField(title));
    fields.push_back(createLabelField("Username:", USERNAME_LABEL_OFFSET_Y));
    usernameInputField = createInputField(USERNAME_INPUT_OFFSET_Y);
    fields.push_back(usernameInputField);
    fields.push_back(createLabelField("Password:", PASSWORD_LABEL_OFFSET_Y));
    passwordInputField = createInputField(PASSWORD_INPUT_OFFSET_Y);
    fields.push_back(passwordInputField);
    messageField = createMessageField();
    fields.push_back(messageField);
    loadingField = createLoadingField();
    fields.push_back(loadingField);
    fields.push_back(nullptr);

    form = new_form(fields.data());
    set_form_win(form, entryWindow);
    set_form_sub(form, entrySubWindow);
    post_form(form);
}

void AuthenticationWindow::setOnCancel(std::function<void()> callback) {
    onCancelCallback = callback;
}

void AuthenticationWindow::setOnInput(std::function<void(const std::string& username, const std::string& password)> callback) {
    onInputCallback = callback;
}

void AuthenticationWindow::showMessage(const std::string& message) {
    set_field_buffer(messageField, 0, message.c_str());
    set_field_buffer(loadingField, 0, "");
    isLoading = false;
}

void AuthenticationWindow::processCredentials() {
    if(!onInputCallback) {
        showMessage("error");
        return;
    }

    auto username =  std::string{field_buffer(usernameInputField, 0), FIELD_WIDTH};
    boost::trim(username);
    if (username.empty()) {
        showMessage("Must enter username");
        return;
    }

    if (password.empty()) {
        showMessage("Must enter password");
        return;
    }

    onInputCallback(username, password);
    showMessage("Connecting...");
    isLoading = true;
}

void AuthenticationWindow::onEnter() {
    Window::onEnter();
    curs_set(true);
    pos_form_cursor(form);
}

void AuthenticationWindow::onExit() {
    Window::onExit();
    showMessage("");
    isLoading = false;
}

void AuthenticationWindow::updatePasswordField(bool characterAdded) {
    form_driver(form, REQ_VALIDATION);
    //delete chracter from password
    if( !(characterAdded || password.empty()) ) {
        password.pop_back();
        redrawPasswordField();
        return;
    }

    auto fieldBuff = std::string{field_buffer(passwordInputField, 0), FIELD_WIDTH};
    boost::trim(fieldBuff);
    if (fieldBuff.empty()) {
        return;
    }

    auto lastChar = fieldBuff.back();
    password += lastChar;

    redrawPasswordField();
}

void AuthenticationWindow::redrawPasswordField() {
    set_field_buffer(passwordInputField, 0, passwordFill.substr(0, password.length()).c_str());
    form_driver(form, REQ_END_FIELD);
}

}