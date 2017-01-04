#include "WorldBuildingWindow.hpp"
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>
#include <StringUtils.hpp>

namespace {
const std::string AREA_INFO_ITEM_NAME = "Info";
const std::string EXITS_ITEM_NAME = "Exits";
const std::string OBJECTS_ITEM_NAME = "Objects";
const std::string SUBMIT_ITEM_NAME = "Submit";
const std::string DISCARD_ITEM_NAME = "Discard";
const std::vector<std::string> AREA_MENU_ITEMS{
        AREA_INFO_ITEM_NAME,
        EXITS_ITEM_NAME,
        OBJECTS_ITEM_NAME,
        SUBMIT_ITEM_NAME,
        DISCARD_ITEM_NAME
};

const int MAIN_MENU_WIDTH = 18;
const int SECONDARY_MENU_WIDTH = 30;
const int MESSAGE_WINDOW_HEIGHT = 5;

const std::string AREA_INFO_ID_NAME = "ID";
const std::string AREA_INFO_TITLE_NAME = "Name";
const std::string AREA_INFO_DESC_NAME = "Description";
const std::vector<std::string> AREA_INFO_MENU_ITEMS{AREA_INFO_ID_NAME, AREA_INFO_TITLE_NAME, AREA_INFO_DESC_NAME};

const std::string EXITS_MENU_ADD = "Add exit";
const std::string OBJECTS_MENU_ADD = "Add Object";

MENU* createMenu(const std::vector<std::string>& itemNames, std::vector<ITEM*>& items,
                 WINDOW* menuWindow, WINDOW* menuSubWindow) {
    items.reserve(itemNames.size() + 1);
    for (const auto& name : itemNames) {
        auto cName = name.c_str();
        items.emplace_back(new_item(cName, cName));
    }
    items.emplace_back(nullptr);

    MENU* menu = new_menu(items.data());
    set_menu_win(menu, menuWindow);
    set_menu_sub(menu, menuSubWindow);
    set_menu_mark(menu, "");
    set_menu_fore(menu, 0);
    menu_opts_off(menu, O_SHOWDESC);

    post_menu(menu);
    return menu;
}

bool anyTypeFieldFunc(FIELD*, const void*) {
    return true;
}

bool anyTypeCharFunc(int, const void*) {
    return true;
}

}

namespace gui {

WorldBuildingWindow::WorldBuildingWindow() {
    setupWindows();
    setupMenus();
    setupForm();

    TYPE_ANY = new_fieldtype(anyTypeFieldFunc, anyTypeCharFunc);
    switchToMainMenu();
}

void WorldBuildingWindow::update(int input) {
    if (isEditing()) {
        handleInputForForm(input);
        return;
    }

    switch (input) {
        case '\n':
        case KEY_ENTER:
        case KEY_RIGHT:
            handleSelect();
            break;
        case KEY_LEFT:
        case 27:
            if (currentFocusedMenu == mainMenu) {
                break;
            }
            handleCancel();
            break;
        case KEY_UP:
            menu_driver(currentFocusedMenu, REQ_UP_ITEM);
            showSecondaryMenu();
            break;
        case KEY_DOWN:
            menu_driver(currentFocusedMenu, REQ_DOWN_ITEM);
            showSecondaryMenu();
            break;
        default:
            break;
    }
}

void WorldBuildingWindow::redraw() {
    //ncurses wont print elsewhere for some reason
    box(messageWindow, 0, 0);
    box(mainMenuWindow, 0, 0);

    if (currentFocusedMenu == currentSecondaryMenu) {
        box(secondaryMenuWindow, 0, 0);
    } else {
        box(secondaryMenuWindow, ' ', ' ');
    }

    if (isEditing()) {
        box(formSubWindow, 0, 0);
    }

    wrefresh(mainWindow);
}

void WorldBuildingWindow::resize(const Size& maxSize) {

}

WINDOW* WorldBuildingWindow::getCursesWindow() {
    return focusedWindow;
}

void WorldBuildingWindow::setOnSubmit(std::function<void(const Area& node)>  callback) {
    onSubmitCallback = callback;
}

void WorldBuildingWindow::setOnQuit(std::function<void()> callback) {
    onQuitCallback = callback;
}

void WorldBuildingWindow::setOnDiscard(std::function<void()> callback) {
    onDiscardCallback = callback;
}

void WorldBuildingWindow::onEnter() {
    Window::onEnter();
    curs_set(false);
    switchToMainMenu();
    redrawMenu(mainMenu);
    redrawMenu(currentSecondaryMenu);
}

void WorldBuildingWindow::onExit() {
    Window::onExit();
}

void WorldBuildingWindow::showSecondaryMenu() {
    unpost_menu(currentSecondaryMenu);

    switch (getCurrentMainMenuItem()) {
        case MainMenuItem::INFO:
            currentSecondaryMenu = areaInfoMenu;
            break;
        case MainMenuItem::EXITS:
            currentSecondaryMenu = exitsMenu;
            break;
        default:
            currentSecondaryMenu = nullptr;
            break;
    }

    post_menu(currentSecondaryMenu);
}

void WorldBuildingWindow::switchToSecondaryMenu() {
    currentFocusedMenu = currentSecondaryMenu;
    menu_driver(currentSecondaryMenu, REQ_FIRST_ITEM);
    set_menu_fore(currentSecondaryMenu, A_STANDOUT);
    set_menu_fore(mainMenu, 0);
}

void WorldBuildingWindow::switchToMainMenu() {
    currentFocusedMenu = mainMenu;
    set_menu_fore(currentSecondaryMenu, 0);
    set_menu_fore(mainMenu, A_STANDOUT);
}

WorldBuildingWindow::MainMenuItem WorldBuildingWindow::getCurrentMainMenuItem() {
    static std::unordered_map<std::string, MainMenuItem> mainMenuItemMap = {
            {AREA_INFO_ITEM_NAME, MainMenuItem::INFO},
            {EXITS_ITEM_NAME, MainMenuItem::EXITS},
            {OBJECTS_ITEM_NAME, MainMenuItem::OBJECTS},
            {SUBMIT_ITEM_NAME, MainMenuItem::SUBMIT},
            {DISCARD_ITEM_NAME, MainMenuItem::DISCARD}
    };

    auto mainMenuItem = current_item(mainMenu);
    auto mainMenuItemName = item_name(mainMenuItem);
    CHECK(mainMenuItemMap.find(mainMenuItemName) != mainMenuItemMap.end()) << "No item : " << mainMenuItemName;
    return mainMenuItemMap.at(mainMenuItemName);
}

void WorldBuildingWindow::handleSelect() {
    if (currentFocusedMenu == mainMenu) {
        //Handle main menu options
        switch (getCurrentMainMenuItem()) {
            case MainMenuItem::SUBMIT:
                if (onSubmitCallback && currentArea.is_initialized()) {
                    onSubmitCallback(currentArea.get());
                }
                break;
            case MainMenuItem::DISCARD:
                if (onDiscardCallback) {
                    onDiscardCallback();
                }
                break;
            case MainMenuItem::OBJECTS:
                break; //Nothing for now
            default:
                switchToSecondaryMenu();
                break;
        }

        return;
    }

    //Handle secondary menu options
    handleSecondaryMenuSelect(getCurrentMainMenuItem());
}

void WorldBuildingWindow::handleSecondaryMenuSelect(MainMenuItem mainMenuItem) {
    auto itemName = getSecondaryMenuItem();
    //Set the type of value we are going to edit
    switch (mainMenuItem) {
        case MainMenuItem::INFO:
            if (itemName == AREA_INFO_ID_NAME) {
                currentEditType = AreaEditType::ID;
            } else if (itemName == AREA_INFO_TITLE_NAME) {
                currentEditType = AreaEditType::NAME;
            } else {
                currentEditType = AreaEditType::DESC;
            }
            break;
        case MainMenuItem::EXITS:
            if (getSecondaryMenuItem() == EXITS_MENU_ADD) {
                currentEditType = AreaEditType::NEW_EXIT;
                break;
            }

            currentEditType = AreaEditType::EXIT;
            break;
        case MainMenuItem::OBJECTS:
            if (getSecondaryMenuItem() == OBJECTS_MENU_ADD) {
                currentEditType = AreaEditType::NEW_OBJECT;
                break;
            }

            currentEditType = AreaEditType::OBJECT;
            break;
        default:
            LOG(ERROR) << "Not a valid secondary menu item";
    }

    focusedWindow = formWindow;
    showForm();
}

std::string WorldBuildingWindow::getSecondaryMenuItem() {
    auto item = current_item(currentSecondaryMenu);
    return item_name(item);
}

void WorldBuildingWindow::handleCancel() {
    if (isEditing()) {
        //Exit from editing form
        hideForm();
        switchToSecondaryMenu();
        return;
    }

    if (currentFocusedMenu == mainMenu) {
        if (onQuitCallback) {
            onQuitCallback();
        }

        return;
    }

    //secondary window is currently focused
    switchToMainMenu();
}

void WorldBuildingWindow::handleInputForForm(int val) {
    if (!isEditing()) {
        return;
    }

    switch (val) {
        case '\n':
        case KEY_ENTER:
            form_driver(editForm, REQ_END_FIELD);
            form_driver(editForm, REQ_VALIDATION);
            if (submitForm()) {
                handleCancel();
            }
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(editForm, REQ_DEL_PREV);
            form_driver(editForm, REQ_END_FIELD);
            break;
        case 27:
            handleCancel();
            break;
        case KEY_UP:
            form_driver(editForm, REQ_PREV_FIELD);
            form_driver(editForm, REQ_END_FIELD);
            break;
        case KEY_DOWN:
            form_driver(editForm, REQ_NEXT_FIELD);
            form_driver(editForm, REQ_END_FIELD);
            break;
        case KEY_DC:
            set_field_buffer(editFields[1], 0, "");
            break;
        default:
            form_driver(editForm, val);
            break;
    }
}

void WorldBuildingWindow::showForm() {
    curs_set(true);
    unpost_form(editForm);

    //Show the form with the current value
    FIELD* title = editFields[0];
    FIELD* value = editFields[1];

    CHECK(currentArea) << "Should have area data";
    Area& area = currentArea.get();
    switch (currentEditType.get()) {
        case AreaEditType::ID:
            field_opts_off(title, O_EDIT | O_ACTIVE);
            field_opts_off(value, O_EDIT | O_ACTIVE);

            set_field_buffer(title, 0, "AREA ID");
            set_field_buffer(value, 0, area.getID().c_str());
            break;
        case AreaEditType::NAME:
            set_field_type(value, TYPE_ANY);
            field_opts_off(title, O_EDIT | O_ACTIVE);
            field_opts_on(value, O_EDIT | O_ACTIVE);

            set_field_buffer(title, 0, "AREA NAME");
            set_field_buffer(value, 0, area.getTitle().c_str());
            break;
        case AreaEditType::DESC:
            set_field_type(value, TYPE_ANY);
            field_opts_on(value, O_EDIT | O_ACTIVE);
            field_opts_off(title, O_EDIT | O_ACTIVE);

            set_field_buffer(title, 0, "AREA DESCRIPTION");
            set_field_buffer(value, 0, area.getDescription().c_str());
            break;
        case AreaEditType::EXIT: {
            field_opts_on(title, O_EDIT | O_ACTIVE);
            field_opts_on(value, O_EDIT | O_ACTIVE);
            set_field_type(title, TYPE_ANY);
            set_field_type(value, TYPE_INTEGER, 0, std::numeric_limits<int>::max());

            auto exitName = getSecondaryMenuItem();
            auto exitTo = currentArea->getConnectedAreas()->at(exitName);

            set_field_buffer(title, 0, exitName.c_str());
            set_field_buffer(value, 0, exitTo.c_str());
            break;
        }
        case AreaEditType::NEW_EXIT:
            field_opts_on(title, O_EDIT | O_ACTIVE);
            field_opts_on(value, O_EDIT | O_ACTIVE);
            set_field_type(title, TYPE_ANY);
            set_field_type(value, TYPE_INTEGER, 0, std::numeric_limits<int>::max());

            set_field_buffer(title, 0, "EXIT NAME");
            set_field_buffer(value, 0, "");
            break;
        case AreaEditType::OBJECT: {
            auto titleStr = "OBJECT " + getSecondaryMenuItem();
            set_field_buffer(title, 0, titleStr.c_str());
            set_field_buffer(value, 0, getSecondaryMenuItem().c_str());
            break;
        }
        case AreaEditType::NEW_OBJECT:
            set_field_buffer(title, 0, "NEW OBJECT");
            break;
    }

    post_form(editForm);
    form_driver(editForm, REQ_FIRST_FIELD);
    form_driver(editForm, REQ_END_FIELD);
}

void WorldBuildingWindow::hideForm() {
    curs_set(false);
    focusedWindow = mainWindow;

    currentEditType.reset();
    set_field_buffer(editFields[0], 0, "");
    set_field_buffer(editFields[1], 0, "");
    unpost_form(editForm);
}

bool WorldBuildingWindow::submitForm() {
    auto value = std::string{field_buffer(editFields[1], 0)};
    boost::algorithm::trim(value);

    CHECK(currentArea) << "Current area isn't set";
    CHECK(isEditing()) << "Not editing form";
    Area& area = currentArea.get();
    switch (currentEditType.get()) {
        case AreaEditType::ID:
            break;
        case AreaEditType::NAME:
            area.setTitle(value);
            break;
        case AreaEditType::DESC:
            area.setDescription(value);
            break;
        case AreaEditType::EXIT: {
            //remove old exit
            auto currExit = getSecondaryMenuItem();
            currentArea->removeConnectedArea(currExit);
            auto it = std::find(exitsMenuItemNames.begin(), exitsMenuItemNames.end(), currExit);
            if (it != exitsMenuItemNames.end()) {
                exitsMenuItemNames.erase(it);
            }
            //Fallthrough
        }
        case AreaEditType::NEW_EXIT: {
            FIELD* titleField = editFields[0];
            auto exitName = std::string{field_buffer(titleField, 0)};
            boost::trim(exitName);

            if(exitName.empty() || value.empty()) {
                set_field_buffer(editFields[2], 0, "Cant have empty fields");
                return false;
            }

            if (std::find(exitsMenuItemNames.begin(), exitsMenuItemNames.end(), exitName) != exitsMenuItemNames.end()) {
                set_field_buffer(editFields[2], 0, "Exit already exists");
                return false;
            }

            addNewExitItem(exitName, value);
            area.addConnectedArea(exitName, value);
            field_opts_off(titleField, O_EDIT | O_ACTIVE);
            redrawMenu(exitsMenu);
            break;
        }
        case AreaEditType::OBJECT:
            break;
        case AreaEditType::NEW_OBJECT:
            break;
    }

    return true;
}

void WorldBuildingWindow::addNewExitItem(const std::string& exit, const std::string& to) {
    //TODO insert wihtout recreating?
    for (auto item : exitsMenuItems) {
        free_item(item);
    }
    free_menu(exitsMenu);

    if (exitsMenuItemNames.empty()) {
        exitsMenuItemNames.emplace_back(exit);
    } else {
        exitsMenuItemNames.back() = exit;
    }

    exitsMenuItems.clear();
    setUpExitsMenu();
    currentSecondaryMenu = exitsMenu;
}

void WorldBuildingWindow::setUpExitsMenu() {
    exitsMenuItemNames.emplace_back(EXITS_MENU_ADD);
    exitsMenuItems.clear();
    exitsMenu = createMenu(exitsMenuItemNames, exitsMenuItems, secondaryMenuWindow, secondaryMenuSubWindow);
}

bool WorldBuildingWindow::isEditing() {
    return currentEditType.is_initialized();
}

void WorldBuildingWindow::loadAreaData(const Area& area) {
    currentArea = area;

    exitsMenuItemNames.clear();
    for (const auto& exit : *area.getConnectedAreas()) {
        exitsMenuItemNames.emplace_back(exit.first);
    }

    for (auto item : exitsMenuItems) {
        free_item(item);
    }
    free_menu(exitsMenu);

    setUpExitsMenu();

    menu_driver(currentFocusedMenu, REQ_FIRST_ITEM);
    showSecondaryMenu();
}

void WorldBuildingWindow::redrawMenu(MENU* menu) {
    unpost_menu(menu);
    post_menu(menu);
}

bool WorldBuildingWindow::hasAreaData() const {
    return currentArea.is_initialized();
}

void WorldBuildingWindow::showMessage(const std::string& text) {
    wclear(messageSubWindow);
    wprintw(messageSubWindow, text.c_str());
}

void WorldBuildingWindow::setupWindows() {
    mainWindow = newwin(LINES, COLS, 0, 0);
    keypad(mainWindow, true);
    focusedWindow = mainWindow;
    CHECK(mainWindow) << "Error creating main window";

    messageWindow = derwin(mainWindow, MESSAGE_WINDOW_HEIGHT, COLS, 0, 0);
    CHECK(messageWindow) << "Error creating message window";
    messageSubWindow = derwin(messageWindow, MESSAGE_WINDOW_HEIGHT - 2, COLS - 2, 1, 1);
    CHECK(messageSubWindow) << "Error creating message sub window";

    int menuHeight = LINES - MESSAGE_WINDOW_HEIGHT;
    mainMenuWindow = derwin(mainWindow, menuHeight, MAIN_MENU_WIDTH, MESSAGE_WINDOW_HEIGHT, 0);
    keypad(mainMenuWindow, true);
    CHECK(mainMenuWindow) << "Error creating menu window";
    mainMenuSubWindow = derwin(mainMenuWindow, menuHeight - 2, MAIN_MENU_WIDTH - 2, 1, 1);
    CHECK(mainMenuSubWindow) << "Error creating sub window";

    secondaryMenuWindow = derwin(mainWindow, menuHeight, SECONDARY_MENU_WIDTH, MESSAGE_WINDOW_HEIGHT, MAIN_MENU_WIDTH);
    keypad(secondaryMenuWindow, true);
    CHECK(secondaryMenuWindow) << "Error creating secondary menu window";
    secondaryMenuSubWindow = derwin(secondaryMenuWindow, menuHeight - 2, SECONDARY_MENU_WIDTH - 2, 1, 1);
    CHECK(secondaryMenuSubWindow) << "Error creating secondary sub menu window";

    int formWdith = COLS - SECONDARY_MENU_WIDTH - MAIN_MENU_WIDTH;
    formWindow = derwin(mainWindow, menuHeight, formWdith, MESSAGE_WINDOW_HEIGHT, MAIN_MENU_WIDTH + SECONDARY_MENU_WIDTH);
    keypad(formWindow, true);
    CHECK(formWindow) << "Error creating form window";
    formSubWindow = derwin(formWindow, menuHeight, formWdith, 0, 0);
    CHECK(formSubWindow) << "Error creating sub form window";
}

void WorldBuildingWindow::setupMenus() {
    mainMenu = createMenu(AREA_MENU_ITEMS, mainMenuItems,mainMenuWindow, mainMenuSubWindow);
    currentFocusedMenu = mainMenu;

    setUpExitsMenu();

    areaInfoMenu = createMenu(AREA_INFO_MENU_ITEMS, areaInfoMenuItems, secondaryMenuWindow, secondaryMenuSubWindow);
    currentSecondaryMenu = areaInfoMenu;
}

void WorldBuildingWindow::setupForm() {
    int w;
    int h;
    getmaxyx(formSubWindow, h, w);

    auto titleField = new_field(1, w - 2, 1, 1, 0, 0);
    field_opts_off(titleField, O_AUTOSKIP | O_ACTIVE | O_EDIT);
    set_field_opts(titleField, O_STATIC | O_PUBLIC | O_VISIBLE);
    set_field_fore(titleField, A_BOLD | A_UNDERLINE);
    editFields[0] = titleField;

    auto valueField = new_field(h - 4, w - 2, 2, 1, 0, 0);//-2 for title field and error field -2 for border
    field_opts_off(valueField, O_AUTOSKIP);
    set_field_opts(valueField, O_STATIC | O_PUBLIC | O_VISIBLE | O_ACTIVE | O_EDIT | O_WRAP);
    editFields[1] = valueField;

    auto errorField = new_field(1, w - 2, h - 2, 1, 0, 0);
    field_opts_off(errorField, O_AUTOSKIP | O_ACTIVE | O_EDIT);
    set_field_opts(errorField, O_STATIC | O_PUBLIC | O_VISIBLE);
    editFields[2] = errorField;

    editForm = new_form(editFields);
    set_form_win(editForm, formWindow);
    set_form_sub(editForm, formSubWindow);
}
}