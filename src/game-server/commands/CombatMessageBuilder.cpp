#include "CombatMessageBuilder.hpp"
#include "DisplayMessageBuilder.hpp"
#include <game/GameModel.hpp>

CombatMessageBuilder::CombatMessageBuilder(const protocols::CombatInfo& info, const networking::Connection& player)
        : info{info},
          player{player}
{}

std::vector<MessageInfo> CombatMessageBuilder::buildMessages() const {
    std::vector<MessageInfo> messages;
    messages.emplace_back(MessageInfo{client : player, message : protocols::createCombatInfoResponse(info)});

    return messages;
}

void CombatMessageBuilder::addTarget(const networking::Connection& target) {
    this->target.emplace(target);
}
