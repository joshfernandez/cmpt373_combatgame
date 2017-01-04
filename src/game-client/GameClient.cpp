/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include <glog/logging.h>
#include "networking/client.h"
#include "StringUtils.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "game/protocols/Authentication.hpp"
#include "game/protocols/PlayerCommand.hpp"
#include "game/protocols/DisplayMessage.hpp"
#include "game/protocols/CommandInfo.hpp"
#include "game/protocols/Editing.hpp"
#include "game/GameDataImporter.hpp"
#include "Application.hpp"
#include "MainMenuWindow.hpp"
#include "AuthenticationWindow.hpp"
#include "ChatWindow.hpp"
#include "WorldBuildingWindow.hpp"
#include "CombatWindow.hpp"
#include "ArtWindow.hpp"
#include <chrono>
#include <thread>

using namespace networking;

static const std::string MAIN_MENU_WINDOW_ID = "auth";
static const std::string LOGIN_WINDOW_ID = "login";
static const std::string REGISTER_WINDOW_ID = "register";
static const std::string CHAT_WINDOW_ID = "chat";
static const std::string COMBAT_WINDOW_ID = "combat";
static const std::string WORLDBUILDING_WINDOW_ID = "build";
static const std::string ART_WINDOW_ID = "art";
bool running = true;

std::unique_ptr<Client> networkingClient;
std::unique_ptr<gui::Application> gameClient;
std::unique_ptr<gui::MainMenuWindow> authWindow;
std::unique_ptr<gui::AuthenticationWindow> loginWindow;
std::unique_ptr<gui::AuthenticationWindow> registerWindow;
std::unique_ptr<gui::ChatWindow> chatWindow;
std::unique_ptr<gui::WorldBuildingWindow> worldBuildingWindow;
std::unique_ptr<gui::CombatWindow> combatWindow;
std::unique_ptr<gui::ArtWindow> artWindow;

std::string editCommand;
std::string castCommand;

void setupAuthWindow() {
    authWindow = std::make_unique<gui::MainMenuWindow>();
    authWindow->setOnSelection([](auto selection){
        switch(selection) {
            case gui::MainMenuChoice::LOGIN:
                gameClient->switchToWindow(LOGIN_WINDOW_ID);
                break;
            case gui::MainMenuChoice::REGISTER:
                gameClient->switchToWindow(REGISTER_WINDOW_ID);
                break;
            case gui::MainMenuChoice::EXIT:
                running = false;
                break;
            default:
                LOG(FATAL) << "No case for choice";
        }
    });

    gameClient->addWindow(MAIN_MENU_WINDOW_ID, authWindow.get());
}

void setupLoginWindow() {
    loginWindow = std::make_unique<gui::AuthenticationWindow>("LOGIN");

    loginWindow->setOnCancel([](){
        gameClient->switchToWindow(MAIN_MENU_WINDOW_ID);
    });

    loginWindow->setOnInput([](auto user, auto password) {
        auto requestMessage = protocols::createLoginRequestMessage({user, password});
        networkingClient->send(protocols::serializeRequestMessage(requestMessage));
    });

    gameClient->addWindow(LOGIN_WINDOW_ID, loginWindow.get());
}

void setupRegisterWindow() {
    registerWindow = std::make_unique<gui::AuthenticationWindow>("REGISTER");

    registerWindow->setOnCancel([](){
        gameClient->switchToWindow(MAIN_MENU_WINDOW_ID);
    });

    registerWindow->setOnInput([](auto username, auto password) {
        auto requestMessage = protocols::createRegistrationRequestMessage({username, password});
        networkingClient->send(protocols::serializeRequestMessage(requestMessage));
    });

    gameClient->addWindow(REGISTER_WINDOW_ID, registerWindow.get());
}

void setupChatWindow() {
    chatWindow = std::make_unique<gui::ChatWindow>();

    chatWindow->setOnInput([](auto inputText){
        auto separated = separateFirstWord(inputText);
        auto request = protocols::createPlayerCommandRequestMessage({separated.first, separated.second});
        networkingClient->send(protocols::serializeRequestMessage(request));
    });

    gameClient->addWindow(CHAT_WINDOW_ID, chatWindow.get());
}

void setupWorldBuildingWindow() {
    worldBuildingWindow = std::make_unique<gui::WorldBuildingWindow>();

    worldBuildingWindow->setOnSubmit([](auto area) {
        YAML::Emitter em;
        em << YAML::Node{area};
        std::stringstream arguments;
        arguments << "submit @" << em.c_str() << "@";
        auto args = arguments.str();
        auto cmd =  protocols::createPlayerCommandRequestMessage(protocols::PlayerCommand{command : editCommand, arguments : args});
        networkingClient->send(protocols::serializeRequestMessage(cmd));
    });

    worldBuildingWindow->setOnQuit([]() {
       gameClient->switchToWindow(CHAT_WINDOW_ID);
    });

    worldBuildingWindow->setOnDiscard([]() {
        auto cmd =  protocols::createPlayerCommandRequestMessage(protocols::PlayerCommand{command : editCommand, arguments : "cancel"});
        networkingClient->send(protocols::serializeRequestMessage(cmd));
        gameClient->switchToWindow(CHAT_WINDOW_ID);
    });

    worldBuildingWindow->setOnQuit([]() {
        gameClient->switchToWindow(CHAT_WINDOW_ID);
    });

    gameClient->addWindow(WORLDBUILDING_WINDOW_ID, worldBuildingWindow.get());
}

void setupCombatWindow() {
    combatWindow = std::make_unique<gui::CombatWindow>();

    combatWindow->setOnSelection([](const auto& spellName) {
        auto castArgs = spellName + " " + combatWindow->getTargetName();
        auto cmd =  protocols::createPlayerCommandRequestMessage(
                protocols::PlayerCommand{
                        command : castCommand,
                        arguments : castArgs
                });
        networkingClient->send(protocols::serializeRequestMessage(cmd));
    });

    gameClient->addWindow(COMBAT_WINDOW_ID, combatWindow.get());
}

void setupArtWindow() {
    artWindow = std::make_unique<gui::ArtWindow>();
    gameClient->addWindow(ART_WINDOW_ID, artWindow.get());
}

void handleAuthResponse(const protocols::ResponseMessage& response) {
    auto authResponse = protocols::readAuthenticationResponseMessage(response);
    if(authResponse.success) {
        auto cmd = protocols::createCommandInfoRequest(protocols::CommandName::EDIT);
        networkingClient->send(protocols::serializeRequestMessage(cmd));

        auto cast = protocols::createCommandInfoRequest(protocols::CommandName::CAST);
        networkingClient->send(protocols::serializeRequestMessage(cast));

        gameClient->switchToWindow(CHAT_WINDOW_ID);
        chatWindow->showText("Welcome!");
        return;
    }

    auto currentWindowName = gameClient->getCurrentWindowName();
    if (currentWindowName == LOGIN_WINDOW_ID) {
        loginWindow->showMessage(authResponse.msg);
    } else {
        registerWindow->showMessage(authResponse.msg);
    }
}

void handleDisplayResponse(const protocols::ResponseMessage& responseMessage) {
    auto displayMessage = protocols::readDisplayResponseMessage(responseMessage);

    std::string displayText;
    if (displayMessage.sender) {
        displayText += "[" + displayMessage.sender.get() + "] - ";
    }
    displayText += displayMessage.message;

    if (gameClient->getCurrentWindowName() == COMBAT_WINDOW_ID) {
        combatWindow->appendText(displayText);
    }

    chatWindow->showText(displayText);
}

void handleCommandResponse(const protocols::ResponseMessage& responseMessage) {
    auto response = protocols::readCommandInfoResponse(responseMessage);
    switch(response.name) {
        case protocols::CommandName::EDIT:
            editCommand = response.inputBinding;
            break;
        case protocols::CommandName::CAST:
            castCommand = response.inputBinding;
            break;
        default:
            break;
    }
}

void handleEditResponse(const protocols::ResponseMessage& responseMessage) {
    auto editResponse = protocols::readEditResponse(responseMessage);
    switch (editResponse.editType) {
        case protocols::EditType::AREA: {
            CHECK(editResponse.data) << "No data for area";
            auto area = editResponse.data.get();
            worldBuildingWindow->loadAreaData(area);

            gameClient->switchToWindow(WORLDBUILDING_WINDOW_ID);
            break;
        }
        case protocols::EditType::SAVE_CHANGES: {
            if (!editResponse.success) {
                worldBuildingWindow->showMessage(editResponse.message);
                break;
            }

            gameClient->switchToWindow(CHAT_WINDOW_ID);
            chatWindow->showText(editResponse.message);
            break;
        }
    }
}

void handleCombatInfoResponse(const protocols::ResponseMessage& responseMessage) {
    auto combatInfo = protocols::readCombatInfoResponse(responseMessage);
    switch (combatInfo.status) {
        case protocols::CombatStatus::START:
            //not used right now..
            combatWindow->beginCombat(combatInfo);
            gameClient->switchToWindow(COMBAT_WINDOW_ID);
            break;
        case protocols::CombatStatus::UPDATE:
            combatWindow->updateCombat(combatInfo);
            gameClient->switchToWindow(COMBAT_WINDOW_ID);
            break;
        case protocols::CombatStatus::END:
            if (combatInfo.player->health <= 0) {
                gameClient->switchToWindow(ART_WINDOW_ID);
                artWindow->showAnimation("defeat");
            } else {
                gameClient->switchToWindow(ART_WINDOW_ID);
                artWindow->showAnimation("victory");
            }

            combatWindow->endCombat();
            gameClient->switchToWindow(CHAT_WINDOW_ID);
            break;
    }
}

void updateFromServer() {
    if (networkingClient->isDisconnected()) {
        running = false;
        return;
    }

    try {
        networkingClient->update();
    } catch (std::exception& e) {
        LOG(FATAL) << e.what();
    }

    auto incomingMessages = networkingClient->receive();
    if (incomingMessages.empty()) {
        return;
    }

    //convert text back to response messages
    std::vector<protocols::ResponseMessage> incomingResponses;
    incomingResponses.reserve(incomingMessages.size());
    std::transform(incomingMessages.begin(), incomingMessages.end(), std::back_inserter(incomingResponses),
                   [](auto& message) {
                       return protocols::deserializeResponseMessage(message);
                   });

    for(const auto& response : incomingResponses) {
        switch(response.header){
            case protocols::ResponseHeader::LOGIN_RESPONSE:
            case protocols::ResponseHeader::REGISTER_RESPONSE:
                handleAuthResponse(response);
                break;
            case protocols::ResponseHeader::DISPLAY_MESSAGE_RESPONSE:
                handleDisplayResponse(response);
                break;
            case protocols::ResponseHeader::COMMAND_INFO_RESPONSE:
                handleCommandResponse(response);
                break;
            case protocols::ResponseHeader::EDIT_INFO_RESPONSE:
                handleEditResponse(response);
                break;
            case protocols::ResponseHeader::COMBAT_INFO_RESPONSE:
                handleCombatInfoResponse(response);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n%s <ip address> <port>\ne.g. %s localhost 4000\n",
               argv[0], argv[0]);
        return 1;
    }

    FLAGS_log_dir = "./";
    google::InitGoogleLogging("GameClientLogs");

    gameClient = std::make_unique<gui::Application>();
    setupAuthWindow();
    setupLoginWindow();
    setupRegisterWindow();
    setupChatWindow();
    setupWorldBuildingWindow();
    setupCombatWindow();
    setupArtWindow();

    networkingClient = std::make_unique<Client>(argv[1], argv[2]);

    gameClient->switchToWindow(ART_WINDOW_ID);
    artWindow->playSplash("splash");
    gameClient->switchToWindow(MAIN_MENU_WINDOW_ID);

    while(running) {
        gameClient->update();
        updateFromServer();
    }

    return 0;
}