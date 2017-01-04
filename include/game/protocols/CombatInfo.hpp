#ifndef ADVENTURE2016_COMBATINFO_HPP
#define ADVENTURE2016_COMBATINFO_HPP

#include <game/Spell.hpp>
#include <game/GameModel.hpp>
#include "ResponseMessage.hpp"
#include <boost/optional.hpp>

namespace protocols {

struct CharacterInfo {
    std::string name;
    int level;
    int health;
    int maxHealth;
    int mana;
    int maxMana;
};

enum class CombatStatus {
    START,
    END,
    UPDATE,
};

struct CombatInfo {
    CombatStatus status;
    boost::optional<CharacterInfo> player;
    boost::optional<CharacterInfo> target;
    boost::optional<std::vector<Spell>> playerSpells;
};

ResponseMessage createCombatInfoResponse(const CombatInfo& combatInfo);
CombatInfo readCombatInfoResponse(const ResponseMessage& responseMessage);

static protocols::CharacterInfo getCharacterInfo(const std::string& id, const GameModel& gameModel) {
    protocols::CharacterInfo info;

    auto playerCharacter = gameModel.getCharacterByID(id);

    info.name = id;
    info.level = playerCharacter->getLevel();
    info.health = playerCharacter->getCurrentHealth();
    info.maxHealth = playerCharacter->getMaxHealth();
    info.mana = playerCharacter->getCurrentMana();
    info.maxMana = playerCharacter->getMaxMana();

    return info;
}

}

#endif //ADVENTURE2016_COMBATINFO_HPP
