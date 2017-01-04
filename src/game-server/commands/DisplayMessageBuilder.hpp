#ifndef ADVENTURE2016_DISPLAYMESSAGEBUILDER_HPP
#define ADVENTURE2016_DISPLAYMESSAGEBUILDER_HPP

#include <boost/optional.hpp>
#include <memory>
#include <gsl/gsl>
#include "networking/server.h"
#include "game/protocols/DisplayMessage.hpp"
#include "MessageBuilder.hpp"

/*
 * Builds messages that will be displayed on the client
 */
class DisplayMessageBuilder : public MessageBuilder {
public:
    static const std::string SENDER_SERVER;
    DisplayMessageBuilder(std::string message) : message{std::move(message)} {};

    operator std::unique_ptr<MessageBuilder>();

    DisplayMessageBuilder& addClient(const networking::Connection& client);
    DisplayMessageBuilder& addClients(const gsl::span<networking::Connection, -1> clients);
    DisplayMessageBuilder& setSender(const std::string& sender);

    virtual std::vector<MessageInfo> buildMessages() const override ;
private:
    std::vector<networking::Connection> clientList;
    std::string message;
    boost::optional<std::string> sender;
};


#endif //ADVENTURE2016_DISPLAYMESSAGEBUILDER_HPP
