#ifndef ADVENTURE2016_COMMAND_H
#define ADVENTURE2016_COMMAND_H


#include <memory>
#include <string>
#include "networking/server.h"
#include "MessageBuilder.hpp"
#include <DisplayMessageBuilder.hpp>
#include <gsl/gsl>

struct PlayerInfo {
    std::string playerID;
    networking::Connection clientID;
};

class Command {
public:
    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) = 0;
    virtual ~Command() {};

protected:
	std::unique_ptr<MessageBuilder> buildPlayerMessage(const networking::Connection& client, const std::string& message) {
		return DisplayMessageBuilder{message}
				.addClient(client)
				.setSender(DisplayMessageBuilder::SENDER_SERVER);
	}

	const std::string TARGET_SELF = "self";
};


#endif //ADVENTURE2016_COMMAND_H
