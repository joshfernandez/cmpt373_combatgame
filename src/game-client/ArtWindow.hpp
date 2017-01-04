#ifndef ADVENTURE2016_ARTWINDOW_HPP
#define ADVENTURE2016_ARTWINDOW_HPP

#include "Window.hpp"
#include <string>

namespace gui {

class ArtWindow : public Window {
public:
    ArtWindow();

    virtual void update(int input) override;
    virtual void redraw() override;
    virtual void resize(const Size& maxSize) override;
    virtual WINDOW* getCursesWindow() override;

    void playSplash(const std::string& name);
    void showAnimation(const std::string& name);
private:
    void playAnimation(const std::vector<std::string>& frames, int frameTimeMs);
    WINDOW* mainWindow;
    WINDOW* frameWindow;
};

}
#endif //ADVENTURE2016_ARTWINDOW_HPP
