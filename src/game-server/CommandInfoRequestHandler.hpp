#ifndef ADVENTURE2016_DATAREQUESTHANDLER_HPP
#define ADVENTURE2016_DATAREQUESTHANDLER_HPP

#include <game/protocols/CommandInfo.hpp>
#include <networking/server.h>

class MessageIO;
class CommandConfig;

class CommandInfoRequestHandler {
public:
    CommandInfoRequestHandler(MessageIO& messageIO, CommandConfig& commandConfig);
    void handleDataRequest(protocols::CommandName name, const networking::Connection& requester);
private:
    CommandConfig& commandConfig;
    MessageIO& io;
};


#endif //ADVENTURE2016_DATAREQUESTHANDLER_HPP
