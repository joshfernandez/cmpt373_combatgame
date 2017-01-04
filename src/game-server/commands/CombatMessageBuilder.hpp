#ifndef ADVENTURE2016_COMBATMESSAGEBUILDER_HPP
#define ADVENTURE2016_COMBATMESSAGEBUILDER_HPP

#include "MessageBuilder.hpp"
#include <game/protocols/CombatInfo.hpp>
#include <boost/optional.hpp>

class CombatMessageBuilder : public MessageBuilder {
public:
    CombatMessageBuilder(const protocols::CombatInfo& info, const networking::Connection& player);

    void addTarget(const networking::Connection& target);
    virtual std::vector<MessageInfo> buildMessages() const override;

private:
    protocols::CombatInfo info;
    networking::Connection player;
    boost::optional<networking::Connection> target;
};


#endif //ADVENTURE2016_COMBATMESSAGEBUILDER_HPP
