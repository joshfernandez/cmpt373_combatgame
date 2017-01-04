#include "game/protocols/CombatInfo.hpp"
#include <glog/logging.h>

namespace {

const std::string STATUS_KEY = "st";
const std::string PLAYER_KEY = "p";
const std::string TARGET_KEY = "t";
const std::string SPELLS_KEY = "s";

const std::string CHAR_NAME_KEY = "cn";
const std::string CHAR_LEVEL_KEY = "cl";
const std::string CHAR_HP_KEP = "hp";
const std::string CHAR_MAX_HP_KEY = "mhp";
const std::string CHAR_MANA_KEY = "m";
const std::string CHAR_MAX_MANA_KEY = "mm";

const std::string SPELL_NAME_KEY = "spn";
const std::string SPELL_MANA_KEY = "spm";
const std::string SPELL_TYPE_KEY = "spt";
const std::string SPELL_EFFECT_KEY = "spe";
}

using namespace protocols;
namespace YAML {
template<>
struct convert<CharacterInfo> {
    static Node encode(const CharacterInfo& character) {
        Node node;

        node[CHAR_NAME_KEY] = character.name;
        node[CHAR_LEVEL_KEY] = character.level;
        node[CHAR_HP_KEP] = character.health;
        node[CHAR_MAX_HP_KEY] = character.maxHealth;
        node[CHAR_MANA_KEY] = character.mana;
        node[CHAR_MAX_MANA_KEY] = character.maxMana;

        return node;
    }

    static bool decode(const Node& node, CharacterInfo& character) {
        character.name = node[CHAR_NAME_KEY].as<std::string>();
        character.level = node[CHAR_LEVEL_KEY].as<int>();
        character.health = node[CHAR_HP_KEP].as<int>();
        character.maxHealth = node[CHAR_MAX_HP_KEY].as<int>();
        character.mana = node[CHAR_MANA_KEY].as<int>();
        character.maxMana = node[CHAR_MAX_MANA_KEY].as<int>();

        return true;
    }
};

template<>
struct convert<Spell> {
    static Node encode(const Spell& spell) {
        Node node;

        node[SPELL_NAME_KEY] = spell.getName();
        node[SPELL_MANA_KEY] = spell.getManaCost();
        node[SPELL_TYPE_KEY] = static_cast<int>(spell.getType());
        node[SPELL_EFFECT_KEY] = "";

        return node;
    }

    static bool decode(const Node& node, Spell& spell) {
        auto name = node[SPELL_NAME_KEY].as<std::string>();
        auto mana = node[SPELL_MANA_KEY].as<int>();
        auto type = static_cast<SpellType>(node[SPELL_TYPE_KEY].as<int>());
        spell = Spell{name, mana, type, ""};

        return true;
    }
};

}

namespace protocols {

ResponseMessage createCombatInfoResponse(const CombatInfo& combatInfo) {
    YAML::Node body;
    body[STATUS_KEY] = static_cast<int>(combatInfo.status);

    switch (combatInfo.status) {
        case CombatStatus::START:
        case CombatStatus::UPDATE:
            CHECK(combatInfo.playerSpells) << "Require spells";
            body[SPELLS_KEY] = combatInfo.playerSpells.get();
            CHECK(combatInfo.target) << "Require target";
            body[TARGET_KEY] = combatInfo.target.get();
            //Fallthrough
        case CombatStatus::END:
            CHECK(combatInfo.player) << "Require player";
            body[PLAYER_KEY] = combatInfo.player.get();
            break;
    }

    return ResponseMessage{header : ResponseHeader::COMBAT_INFO_RESPONSE, body : body};
}

CombatInfo readCombatInfoResponse(const ResponseMessage& responseMessage) {
    CHECK(responseMessage.header == ResponseHeader::COMBAT_INFO_RESPONSE) << "Wrong header";

    CombatInfo combatInfo;
    combatInfo.status = static_cast<CombatStatus>(responseMessage.body[STATUS_KEY].as<int>());

    switch (combatInfo.status) {
        case CombatStatus::START:
        case CombatStatus::UPDATE:
            combatInfo.playerSpells.emplace(responseMessage.body[SPELLS_KEY].as<std::vector<Spell>>());
            combatInfo.target.emplace(responseMessage.body[TARGET_KEY].as<CharacterInfo>());
            //Fallthrough
        case CombatStatus::END:
            combatInfo.player.emplace(responseMessage.body[PLAYER_KEY].as<CharacterInfo>());
            break;
    }

    return combatInfo;
}

}