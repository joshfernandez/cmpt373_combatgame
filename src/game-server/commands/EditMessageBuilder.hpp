#ifndef ADVENTURE2016_WORLDBUILDINGMESSAGEBUILDER_HPP
#define ADVENTURE2016_WORLDBUILDINGMESSAGEBUILDER_HPP


#include <game/protocols/Editing.hpp>
#include "MessageBuilder.hpp"

class EditMessageBuilder : public MessageBuilder {
public:
    EditMessageBuilder(const networking::Connection& client, const protocols::EditResponse& editResponse);

    virtual std::vector<MessageInfo> buildMessages() const override;

private:
    networking::Connection client;
    protocols::EditResponse response;
};


#endif //ADVENTURE2016_WORLDBUILDINGMESSAGEBUILDER_HPP
