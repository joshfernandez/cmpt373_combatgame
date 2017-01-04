#ifndef ADVENTURE2016_APPLICATION_HPP
#define ADVENTURE2016_APPLICATION_HPP

#include <unordered_map>
#include <string>
#include "Window.hpp"

namespace gui {

class Application {
public:
    Application();

    virtual ~Application();

    /**
     * @brief Updates the current window and resizes if necessary
     */
    void update();

    /**
     * @brief Switches to window that has the name
     * @param name Name of the window to switch to
     */
    void switchToWindow(const std::string& name);

    /**
     * @brief Adds a window, with the specified name
     * @param name The name of the window so it can be switched to
     * @param window The window to add
     *
     * Application will automatically switch to the first window added
     */
    void addWindow(const std::string& name, Window *window);

    /**
     * Gets the current window name
     * @return The name of the current window
     */
    const std::string& getCurrentWindowName();
private:
    std::unordered_map<std::string, Window*> windows;
    Window* currentWindow = nullptr;
    std::string currentWindowName;
    int w;
    int h;
};

}

#endif //ADVENTURE2016_APPLICATION_HPP
