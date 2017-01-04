#ifndef ADVENTURE2016_WINDOW_HPP
#define ADVENTURE2016_WINDOW_HPP

#include <ncurses.h>

namespace gui {

struct Size {
    int width;
    int height;
};

/**
 * @brief Window interface for ncurses ui
 * The Window class represents an ncurses window. Each window is responsible for redrawing, resizing, and parsing input itself
 */
class Window {
public:
    /**
     * @brief Function to process input
     * @param input The user input
     *
     * All the inputs are valid inputs from the user.
     */
    virtual void update(int input) = 0;

    /**c
     * @brief Function to redraw the window
     */
    virtual void redraw() = 0;

    /**
     * @brief Function to handle resize
     * @param maxSize The max size of the terminal
     *
     * This function will be called when the terminal is resized. When implmenting, you should reposition and resize your
     * windows as neeeded. For example, centering the windows
     */
    virtual void resize(const Size& maxSize) = 0;

    /**
     * @brief Returns the main curses window.
     * @return
     *
     * The window returned will be used
     */
    virtual WINDOW* getCursesWindow() = 0;

    /**
     * @brief Called when the window is switched to
     */
    virtual void onEnter(){};

    /**
     * @brief Called when another window is switched to
     */
    virtual void onExit(){};
    virtual ~Window(){};
};

}


#endif //ADVENTURE2016_WINDOW_HPP
