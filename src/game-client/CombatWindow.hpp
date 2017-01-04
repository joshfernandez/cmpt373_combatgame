#ifndef ADVENTURE2016_COMBATWINDOW_HPP
#define ADVENTURE2016_COMBATWINDOW_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <menu.h>
#include <glog/logging.h>
#include <functional>
#include "Window.hpp"
#include <boost/algorithm/string.hpp>
#include <game/protocols/CombatInfo.hpp>
#include "CharacterAsciis.hpp"

namespace gui {

enum class SpellMenuChoice {
    OFFENSE,
    DEFENSE,
    SUBMENU,
};

class CombatWindow : public Window {
public:
    CombatWindow();
    virtual ~CombatWindow();

    virtual void update(int input) override;
    virtual WINDOW* getCursesWindow() override;
    virtual void resize(const Size& size) override;
    virtual void redraw() override;
    virtual void onEnter() override;

    virtual void onExit() override;

    void setOnSelection(std::function<void(std::string& spellName)> callback);

    void appendText(const std::string& text);
    void beginCombat(const protocols::CombatInfo& combatInfo);
    void updateCombat(const protocols::CombatInfo& info);
    void endCombat();
    std::string getTargetName();
private:
    void createMenu();
    void removeMenu();
    void recreate();
    void refreshMenuSubWindow(gui::SpellMenuChoice menuChoice);
    void clearWindows();

    WINDOW* combatWindow;
    WINDOW* character1Window;
    WINDOW* character2Window;
    WINDOW* character1AsciiWindow;
    WINDOW* character2AsciiWindow;
    WINDOW* messageWindow;
    WINDOW* messageSubWindow;
    WINDOW* menuWindow;
    WINDOW* menuSubWindow;

    std::vector<ITEM*> menuItems;
    MENU* menu;
    Size size;
    std::function<void(std::string& spellName)> callback;

    protocols::CombatInfo combatInfo;
    bool isInCombat = false;
};

}

#endif //ADVENTURE2016_COMBATWINDOW_HPP
