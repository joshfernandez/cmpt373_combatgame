#ifndef ADVENTURE2016_WORLDBUILDINGWINDOW_HPP
#define ADVENTURE2016_WORLDBUILDINGWINDOW_HPP

#include <yaml-cpp/yaml.h>
#include <menu.h>
#include <form.h>
#include <boost/optional.hpp>
#include "Window.hpp"
#include "game/Area.hpp"

namespace gui {

class WorldBuildingWindow : public Window {
public:
    WorldBuildingWindow();

    virtual void update(int input) override;
    virtual void redraw() override;
    virtual void resize(const Size& maxSize) override;
    virtual WINDOW* getCursesWindow() override;
    virtual void onExit() override;
    virtual void onEnter() override;

    void showMessage(const std::string& text);
    void loadAreaData(const Area& area);
    bool hasAreaData() const;

    void setOnSubmit(std::function<void(const Area& area)> callback);
    void setOnQuit(std::function<void()> callback);
    void setOnDiscard(std::function<void()> callback);
private:
    enum class MainMenuItem {
        INFO,
        EXITS,
        OBJECTS,
        SUBMIT,
        DISCARD,
    };

    enum class AreaEditType {
        ID,
        NAME,
        DESC,
        NEW_EXIT,
        EXIT,
        NEW_OBJECT,
        OBJECT,
    };

    MainMenuItem getCurrentMainMenuItem();
    std::string getSecondaryMenuItem();
    void showSecondaryMenu();
    void switchToSecondaryMenu();
    void switchToMainMenu();
    void handleSelect();
    void handleSecondaryMenuSelect(MainMenuItem mainMenuItem);
    void handleCancel();
    void handleInputForForm(int val);
    void showForm();
    void hideForm();
    bool submitForm();
    void addNewExitItem(const std::string& exit, const std::string& to);
    void redrawMenu(MENU* menu);
    bool isEditing();

    void setupWindows();
    void setupMenus();
    void setupForm();
    void setUpExitsMenu();

    WINDOW* focusedWindow;
    WINDOW* mainWindow;
    WINDOW* messageWindow;
    WINDOW* messageSubWindow;
    WINDOW* mainMenuWindow;
    WINDOW* mainMenuSubWindow;
    WINDOW* secondaryMenuWindow;
    WINDOW* secondaryMenuSubWindow;
    WINDOW* formWindow;
    WINDOW* formSubWindow;

    MENU* currentFocusedMenu;
    MENU* currentSecondaryMenu;
    MENU* mainMenu;
    std::vector<ITEM*> mainMenuItems;

    MENU* areaInfoMenu;
    std::vector<ITEM*> areaInfoMenuItems;

    MENU* exitsMenu;
    std::vector<std::string> exitsMenuItemNames;
    std::vector<ITEM*> exitsMenuItems;

    FORM* editForm;
    FIELD* editFields[4] = {nullptr, nullptr, nullptr, nullptr};

    //Cant figure how to reset field type
    FIELDTYPE* TYPE_ANY;
    std::string currentMessage;

    boost::optional<Area> currentArea;
    boost::optional<AreaEditType> currentEditType;

    std::function<void(const Area& area)>  onSubmitCallback;
    std::function<void()> onQuitCallback;
    std::function<void()> onDiscardCallback;
};

}
#endif //ADVENTURE2016_WORLDBUILDINGWINDOW_HPP
