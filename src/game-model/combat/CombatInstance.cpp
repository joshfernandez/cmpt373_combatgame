#include <boost/format.hpp>
#include "CombatInstance.hpp"
#include "GameStrings.hpp"

CombatInstance::CombatInstance(std::unordered_map<std::string, std::shared_ptr<CombatAction>>* actionLookup, std::string defaultActionID)
        : actionLookup(actionLookup), defaultActionID(defaultActionID) {}

void CombatInstance::update() {

    //TODO: Order of execution for character actions
    for (CharacterInstance& characterInstance : characters) {
        if (!characterInstance.isAlive()) {
            continue;
        }
		//TODO: eliminate lookup conditions and replace with assertion, callers will ensure valid strings are passed
        auto iter = actionLookup->find(characterInstance.getCombatActionID());
        if (iter != actionLookup->end()) {
            auto& action = iter->second;
            action->execute(characterInstance.getCharacterRef(), characterInstance.getTarget().getCharacterRef());
        } else {
			auto unknownSpellMessage = GameStrings::getFormatted(GameStringKeys::SPELL_UNKNOWN, StringInfo{"", "", 0, characterInstance.getCombatActionID()});
			characterInstance.getCharacterRef().pushToBuffer(unknownSpellMessage, GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
        }

		//TODO: Extract default case
		if (characterInstance.getCombatActionID() != "attack") {
			characterInstance.setCombatActionID("attack");
		}

    }

}

// A battle is over when there less than two teams have an alive member
bool CombatInstance::isBattleOver() const {

    int firstTeamSeen = -1;

    for (const CharacterInstance& characterInstance : characters) {
        if (characterInstance.isAlive()) {
            int teamID = characterInstance.getTeamID();

            if (firstTeamSeen == -1) {
                // If this is the first team seen, set that as the ID
                firstTeamSeen = teamID;
            } else {
                // If not, and this is a different ID, then there are at least two parties left
                if (firstTeamSeen != teamID) {
                    return false;
                }
            }
        }
    }

    return true;
}

void CombatInstance::battleCleanup() {
    //TODO: Distribute exp, and respawn dead characters
    for (auto& cInstance : characters) {

        Character& character = cInstance.getCharacterRef();
        if (cInstance.isAlive()) {
            // Character is alive, distribute exp and continue
            // TODO: Add actual exp tracking
            character.levelUp();
            character.setState(CharacterState::IDLE);
			character.pushToBuffer(GameStrings::get(GameStringKeys::COMBAT_VICTORY), GameStringKeys::MESSAGE_SENDER_BATTLE, ColorTag::WHITE);
        } else {
            // Character is dead. Set to dead state, GameModel will clean up later.
            // Bug if multiple characters
            character.pushToBuffer(GameStrings::get(GameStringKeys::COMBAT_LOSS), GameStringKeys::MESSAGE_SENDER_BATTLE, ColorTag::WHITE);
        }
    }
}

// TODO: Makes assumptions that teamIDs are added incrementally
int CombatInstance::getOpenTeamID() const {
    if (characters.size() == 0) {
        return 0;
    }

    auto maxTeamID = std::max_element(characters.begin(), characters.end(),
        [] (const auto& a, const auto& b) { return a.getTeamID() < b.getTeamID(); })->getTeamID();
    return maxTeamID + 1;
}

int CombatInstance::addCharacterToNewTeam(Character& character) {
    return addCharacterToTeam(character, getOpenTeamID());
}

int CombatInstance::addCharacterToTeam(Character& character, int teamID) {
    CharacterInstance cInstance(character, defaultActionID, CharacterInstance::nullTarget, teamID);
    characters.push_back(cInstance);
    return teamID;
}

bool CombatInstance::setupInstance() {

    // Initial checks
    if (characters.capacity() <= 1) {
        return false;
    }

    bool bAllAvailable = std::all_of(characters.begin(), characters.end(),
        [] (const auto& c) { return CombatInstance::isAvailable(c); });

    // One of the characters is already in combat, cancel out
    if (!bAllAvailable) {
        return false;
    }

    for (auto& character : characters) {
        character.getCharacterRef().setState(CharacterState::BATTLE);
    }

    // Make sure all characters have a default target
    auto defaultTargets = getDefaultTargets();
    std::vector<int> mapKeys;
    for (const auto& pair : defaultTargets) {
        mapKeys.push_back(pair.first);
    }

    for (auto& character : characters) {
        if (&character.getTarget() == &CharacterInstance::nullTarget) {
            int index = 0;
            while (character.getTeamID() == index) {
                index++;
            }
            character.setTarget(*defaultTargets[index]);
        }
    }

    return true;
}

bool CombatInstance::isAvailable(const CharacterInstance& character) {
    return character.getCharacterRef().getState() == CharacterState::IDLE;
}

std::unordered_map<int, CharacterInstance*> CombatInstance::getDefaultTargets() {
    std::unordered_map<int, CharacterInstance*> defaultTargets;
    for (auto& character : characters) {
        auto teamID = character.getTeamID();
        if (defaultTargets.count(teamID) == 0) {
            defaultTargets[teamID] = &character;
        }
    }
    return defaultTargets;
}

CharacterInstance* CombatInstance::getCharacterInstance(const std::string& characterID) {
    for (auto& characterInstance : characters) {
        if (characterInstance.getCharacterRef().getID() == characterID) {
            return &characterInstance;
        }
    }
    return nullptr;
}

std::vector<std::string> CombatInstance::getPossibleTargets(const std::string& characterID) {
    std::vector<std::string> possibleTargets;
    auto character = getCharacterInstance(characterID);

    if (character == nullptr) {
        return possibleTargets;
    }

    auto teamID = character->getTeamID();
    for (const auto& ci : characters) {
        if (ci.getTeamID() == teamID) {
            continue;
        }
        auto ciID = ci.getCharacterRef().getID();
        if (ciID != characterID) {
            possibleTargets.push_back(ciID);
        }
    }
    return possibleTargets;
}