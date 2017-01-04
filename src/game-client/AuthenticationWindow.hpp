#ifndef ADVENTURE2016_AUTHENTICATIONWINDOW_HPP
#define ADVENTURE2016_AUTHENTICATIONWINDOW_HPP

#include <form.h>
#include <vector>
#include <functional>
#include <string>
#include "Window.hpp"

namespace gui{

class AuthenticationWindow : public Window {
public:
    AuthenticationWindow(std::string title);
    ~AuthenticationWindow();

    virtual void update(int input) override;
    virtual void resize(const Size& maxSize) override;
    virtual WINDOW* getCursesWindow() override;
    virtual void redraw() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    void showMessage(const std::string& message);
    void setOnCancel(std::function<void()> callback);
    void setOnInput(std::function<void(const std::string& username, const std::string& password)> callback);
private:
    void createForm();
    void processCredentials();
    void updatePasswordField(bool characterAdded);
    void redrawPasswordField();

    std::string title;
    std::string password;
    WINDOW* entryWindow;
    WINDOW* entrySubWindow;
    FORM* form;
    std::vector<FIELD*> fields;
    FIELD* usernameInputField;
    FIELD* passwordInputField;
    FIELD* messageField;
    FIELD* loadingField;
    bool isLoading = false;

    std::function<void()> onCancelCallback;
    std::function<void(const std::string& username, const std::string& password)> onInputCallback;
};

}
#endif //ADVENTURE2016_LOGINWINDOW_HPP
