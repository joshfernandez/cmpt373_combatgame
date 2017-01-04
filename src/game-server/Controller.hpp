#ifndef ADVENTURE2016_CONTROLLER_H
#define ADVENTURE2016_CONTROLLER_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <boost/optional.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include "commands/CommandConfig.hpp"
#include "commands/CommandHandle.hpp"
#include "game/protocols/PlayerCommand.hpp"
#include "game/protocols/CommandInfo.hpp"
#include "AccountInfo.hpp"
#include "HelpCommand.hpp"
#include "CommandInfoRequestHandler.hpp"
#include "game/Spell.hpp"

class Command;
class ConnectionManager;
class MessageIO;
class GameModel;

class Controller {
public:
    Controller(GameModel& gameModel, MessageIO& messageIO, ConnectionManager& connectionManager,
                   const CommandConfig& commandCreator);

    void registerCommand(const std::string& commandId, Command& command);
    void processCommand(const protocols::PlayerCommand& command, const networking::Connection& client);
    void processCommandInfoRequest(protocols::CommandName cmdInfoRequest, const networking::Connection& client);

    bool addNewPlayer(const AccountInfo& accountInfo, const networking::Connection& client);
    void removePlayer(const networking::Connection& clientID);
    void disconnectPlayer(const std::string& playerID);
    void update();

    boost::optional<networking::Connection> getClientID(const std::string& playerID) const;
    std::string getPlayerID(const networking::Connection& clientID) const;
    const std::vector<networking::Connection>& getAllClients() const;
    GameModel& getGameModel() const;

private:
    using PlayerMap = boost::bimap<
            boost::bimaps::unordered_set_of<std::string>,
            boost::bimaps::unordered_set_of<networking::Connection, networking::ConnectionHash>
            >;
    using PlayerMapPair = PlayerMap::value_type;

    //bimap from playerID to clientID
    PlayerMap playerMap;
    //keep a list of all connected clients, since its useful when sending messages
    std::vector<networking::Connection> allClients;

    std::unordered_map<std::string, CommandHandle>  inputToCommandMap;
    std::unordered_map<std::string, AccountInfo> playerAccountMap;

    GameModel& gameModel;
    MessageIO& messageIO;
    CommandConfig commandConfig;
    ConnectionManager& connectionManager;
    CommandInfoRequestHandler dataRequestHandler;

    void sendOutput(const MessageBuilder& messageBuilder) const;
    const AccountInfo& getAccountInfo(const networking::Connection& client) const;
    void onCharacterDead(const std::string& playerID);
    std::vector<Spell> spells;
    std::unordered_set<std::string> playersInCombat;

    //help command
    friend class HelpCommand;
    std::unique_ptr<HelpCommand> helpCommand;

    friend class PigLatinDecorator;
    friend class CombatDecorator;
};


#endif //ADVENTURE2016_CONTROLLER_H
