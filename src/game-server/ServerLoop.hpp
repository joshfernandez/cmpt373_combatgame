#ifndef ADVENTURE2016_GAMELOOP_HPP
#define ADVENTURE2016_GAMELOOP_HPP

#include "networking/server.h"
#include "Controller.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "game/protocols/Authentication.hpp"
#include "game/GameDataImporter.hpp"
#include "Authenticator.hpp"
#include "GameFunctions.hpp"
#include "glog/logging.h"
#include "Looper.hpp"
#include "ServerConfig.hpp"
#include "MessageIO.hpp"
#include "ConnectionManager.hpp"

class ServerLoop : public Loop, public MessageIO, public ConnectionManager {
public:
    ServerLoop(const ServerConfig& serverConfig);

    virtual void processInputs(bool& quit) override;
    virtual void update() override;
    virtual void send(const MessageBuilder& messageBuilder) override;

    virtual void disconnectClient(const networking::Connection& connection) override;

    virtual void send(const protocols::CommandInfo& info, const networking::Connection& receiver) override;

private:
    networking::Server server;
    GameModel gameModel;
    Controller controller;
    GameFunctions gameFunctions;
    ServerConfig serverConfig;

    void onConnect(networking::Connection c);
    void onDisconnect(networking::Connection c);
    void processLoginRequest(const protocols::RequestMessage& request, const networking::Connection& clientId);
    void processRegistrationRequest(const protocols::RequestMessage& request, const networking::Connection& clientId);

    void initGameModel(GameModel &gameModel);
};


#endif //ADVENTURE2016_GAMELOOP_HPP
