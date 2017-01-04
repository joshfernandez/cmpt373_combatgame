#include <functional>
#include <iostream>
#include <glog/logging.h>
#include "ServerLoop.hpp"
#include <game/GameModel.hpp>

using namespace networking;

ServerLoop::ServerLoop(const ServerConfig& serverConfig)
        : serverConfig{serverConfig},
          server{serverConfig.getPort(), [this](Connection c){this->onConnect(c);}, [this](Connection c){this->onDisconnect(c);}},
          controller{gameModel, *this, *this, CommandConfig{serverConfig.getCommandConfigFile()}},
          gameFunctions{controller} {
    initGameModel(gameModel);
    std::cout << "Server ready. Listening on port: " << serverConfig.getPort() << std::endl;
}

void ServerLoop::processInputs(bool& quit) {
    try {
        server.update();
    } catch (std::exception& e) {
        LOG(ERROR) << "Exception from Server update:\n" << e.what();
        quit = true;
    }
}

void ServerLoop::update() {
    auto incoming = server.receive();
    for (const auto &clientMessage : incoming) {
        auto requestMessage = protocols::deserializeRequestMessage(clientMessage.text);

        switch (requestMessage.header) {
            case protocols::RequestHeader::LOGIN_REQUEST:
                processLoginRequest(requestMessage, clientMessage.connection);
                break;
            case protocols::RequestHeader::REGISTER_REQUEST:
                processRegistrationRequest(requestMessage, clientMessage.connection);
                break;
            case protocols::RequestHeader::PLAYER_COMMAND_REQUEST: {
                auto playerCommand = protocols::readPlayerCommandRequestMessage(requestMessage);
                controller.processCommand(playerCommand, clientMessage.connection);
                break;
            }
            case protocols::RequestHeader::COMMAND_INFO_REQUEST: {
                auto req = protocols::readCommandInfoRequest(requestMessage);
                controller.processCommandInfoRequest(req, clientMessage.connection);
                break;
            }
            default:
                break;
        }
    }
    gameModel.update();
    controller.update();
}

void ServerLoop::onConnect(Connection c) {
    LOG(INFO) << "New connection found: " << c.id;
}

void ServerLoop::onDisconnect(Connection c) {
    LOG(INFO) << "Connection lost: " << c.id;
    controller.removePlayer(c);
}

void ServerLoop::processLoginRequest(const protocols::RequestMessage& request, const Connection& clientId) {
    auto loginRequest = protocols::readAuthenticationRequestMessage(request);
    auto result = Authenticator::login(loginRequest.username, loginRequest.password);

    auto responseCode = result.result;
    if (responseCode == protocols::LoginResponseCode::LOGIN_OK) {
        if (!controller.addNewPlayer(result.account.get(), clientId)) {
            responseCode = protocols::LoginResponseCode::USER_LOGGED_IN;
        }
    }

    auto loginResponse = protocols::createLoginResponseMessage(responseCode);
    auto output = protocols::serializeResponseMessage(loginResponse);
    server.send(Message{clientId, output});

    //TODO character select. for now skip it and add the player after sucess
}

void ServerLoop::processRegistrationRequest(const protocols::RequestMessage& request, const Connection& clientId) {
    auto registrationRequest = protocols::readAuthenticationRequestMessage(request);
    auto result = Authenticator::registerAccount(registrationRequest.username, registrationRequest.password);

    auto responseCode = result.result;
    if (responseCode == protocols::RegistrationResponseCode::REGISTRATION_OK) {
        auto addOk = controller.addNewPlayer(result.account.get(), clientId);
        CHECK(addOk) << "Could not add player after registering account";
    }

    auto registrationResponse = protocols::createRegistrationResponseMessage(responseCode);
    auto output = protocols::serializeResponseMessage(registrationResponse);
    server.send(Message{clientId, output});
}

void ServerLoop::initGameModel(GameModel& gameModel) {
    auto dataFile = GameDataImporter::getRootYAMLNode(serverConfig.getMapFilePath());

    const YAML::Node NPCS = dataFile["NPCS"];
    gameModel.setNPCs(GameDataImporter::returnNPCS(NPCS));

    const YAML::Node ROOMS = dataFile["ROOMS"];
    auto rooms = GameDataImporter::getRooms(ROOMS);
    for (auto& room: rooms) {
        gameModel.addArea(room);
    }
    gameModel.setDefaultLocationID(rooms[0].getID());

    const YAML::Node OBJECTS = dataFile["OBJECTS"];
    gameModel.loadObjects(OBJECTS);

    const YAML::Node RESETS = dataFile["RESETS"];
	auto resets = GameDataImporter::returnResets(RESETS);
	gameModel.setResets(resets);
	gameModel.addNPCsToAreas();
    gameModel.addObjectToAreas();

    const YAML::Node SHOPS = dataFile["SHOPS"];

    //SPELLS
    auto spellDataFile = GameDataImporter::getRootYAMLNode(serverConfig.getSpellFilePath());
    auto spells = GameDataImporter::getSpells(spellDataFile);
    for (const auto& spell : spells) {
        gameModel.addSpell(spell);
    }
}

void ServerLoop::send(const MessageBuilder& messageBuilder) {
    for (const auto& msg : messageBuilder.buildMessages()) {
        auto serializedResponseMessage = protocols::serializeResponseMessage(msg.message);
        server.send(networking::Message{msg.client, serializedResponseMessage});
    }
}

void ServerLoop::disconnectClient(const Connection& connection) {
    LOG(INFO) << "Disconnecting player: " << connection.id;
    server.disconnect(connection);
}

void ServerLoop::send(const protocols::CommandInfo& info, const Connection& receiver) {
    auto message = protocols::createCommandInfoResponse(info);
    auto responseMessage = protocols::serializeResponseMessage(message);
    server.send(Message{receiver, responseMessage});
}
