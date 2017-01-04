#ifndef ADVENTURE2016_MAINMENUWINDOW_HPP
#define ADVENTURE2016_MAINMENUWINDOW_HPP

#include <memory>
#include <vector>
#include <menu.h>
#include <functional>
#include "Window.hpp"

namespace gui {

enum class MainMenuChoice {
    LOGIN,
    REGISTER,
    EXIT,
};

class MainMenuWindow : public Window {
public:
    MainMenuWindow();
    virtual ~MainMenuWindow();

    virtual void update(int input) override;
    virtual WINDOW* getCursesWindow() override;
    virtual void resize(const Size& size) override;
    virtual void redraw() override;
    virtual void onEnter() override;

    void setOnSelection(std::function<void(MainMenuChoice)> callback);
private:
    void createMenu();
    void removeMenu();
    void recreate();

    WINDOW* menuWindow;
    WINDOW* menuSubWindow;
    std::vector<ITEM*> menuItems;
    MENU* menu;
    Size size;
    std::function<void(MainMenuChoice)> callback;
};

}



#endif //ADVENTURE2016_LOGINWINDOW_HPP
