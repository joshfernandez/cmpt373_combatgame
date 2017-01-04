#include "CharacterInstance.hpp"

Character CharacterInstance::nullCharacter = Character();
CharacterInstance CharacterInstance::nullTarget = CharacterInstance(nullCharacter, "", nullTarget, 0);

CharacterInstance::CharacterInstance(Character& character, const std::string& initialActionID, CharacterInstance& target, int teamID)
        : character(character), selectedActionID(initialActionID), target(&target), teamID(teamID)
{

};

std::string CharacterInstance::getCombatActionID() const {
    return this->selectedActionID;
}

void CharacterInstance::setCombatActionID(const std::string &combatAction) {
    this->selectedActionID = combatAction;
}

CharacterInstance& CharacterInstance::getTarget() {
    return *(this->target);
}

void CharacterInstance::setTarget(CharacterInstance &target) {
    this->target = &target;
}

bool CharacterInstance::isAlive() const {
    return getCharacterRef().getCurrentHealth() > 0;
}

int CharacterInstance::getTeamID() const {
    return this->teamID;
}

void CharacterInstance::setTeamID(const int teamID) {
    this->teamID = teamID;
}

Character& CharacterInstance::getCharacterRef() {
    return this->character;
}

const Character& CharacterInstance::getCharacterRef() const {
    return this->character;
}