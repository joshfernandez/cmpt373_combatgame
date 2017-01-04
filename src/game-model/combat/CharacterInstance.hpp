#ifndef CHARACTERINSTANCE_HPP
#define CHARACTERINSTANCE_HPP

#include <string>
#include <game/Character.hpp>

class CharacterInstance {

public:

    CharacterInstance(Character& character, const std::string& initialActionID, CharacterInstance& target, int teamID);

    static CharacterInstance nullTarget;
    static Character nullCharacter;

    std::string getCombatActionID() const;
    void setCombatActionID(const std::string& combatActionID);

    int getTeamID() const;
    void setTeamID(const int teamID);

    CharacterInstance& getTarget();
    void setTarget(CharacterInstance& target);

    Character& getCharacterRef();
    const Character& getCharacterRef() const;

    bool isAlive() const;

private:

    Character& character;
    int teamID;

    // Action to take on next combat tick
    std::string selectedActionID;
    CharacterInstance* target;
};

#endif