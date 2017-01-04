#include "DisplayMessageBuilder.hpp"
#include <GameStrings.hpp>

const std::string DisplayMessageBuilder::SENDER_SERVER = GameStrings::get(GameStringKeys::SERVER_NAME);

DisplayMessageBuilder& DisplayMessageBuilder::addClient(const networking::Connection& client)
{
    clientList.push_back(client);

    return *this;
}

DisplayMessageBuilder& DisplayMessageBuilder::addClients(const gsl::span<networking::Connection, -1> clients)
{
    clientList.reserve(clientList.size() + clients.size());
    clientList.insert(clientList.end(), clients.begin(), clients.end());

    return *this;
}

DisplayMessageBuilder& DisplayMessageBuilder::setSender(const std::string& sender)
{
    this->sender.emplace(sender);
    return *this;
}

std::vector<MessageInfo> DisplayMessageBuilder::buildMessages() const
{
    std::vector<MessageInfo> output;
    output.reserve(clientList.size());

    std::transform(clientList.begin(), clientList.end(), std::back_inserter(output), [this](const auto& client) {
        protocols::DisplayMessage displayMessage;
        displayMessage.message = message;
        displayMessage.sender = sender;

        MessageInfo msg;
        msg.client = client;
        msg.message = protocols::createDisplayResponseMessage(displayMessage);

        return msg;
    });

    return output;
}

DisplayMessageBuilder::operator std::unique_ptr<MessageBuilder>() {
    return std::make_unique<DisplayMessageBuilder>(*this);
}
