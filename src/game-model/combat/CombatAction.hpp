#ifndef COMBATACTION_HPP
#define COMBATACTION_HPP

#include <string>
#include "CharacterInstance.hpp"
#include "game/Spell.hpp"
#include "GameStrings.hpp"

class CombatAction {

public:

    //TODO: Add constructor for new actions with different effects
    virtual void execute(Character& source, Character& target);
    virtual std::string getID();

    static void dealDamage(Character& character, int amount);
    static void healDamage(Character& character, int amount);

private:

    //TODO: Spell costs/resources/effects

};

class CombatAttack : public CombatAction {

public:

    virtual void execute(Character& source, Character& target);
    virtual std::string getID();

private:

};

class CombatCast : public CombatAction {

public:

    CombatCast(const Spell& spell);
    virtual void execute(Character& source, Character& target);
    virtual std::string getID();
    const Spell& getSpellRef() const;

private:

    const Spell& spell;
	void castOffenseSpell(Character &source, Character &target, int power, const StringInfo &stringInfo) const;
	void castDefenseSpell(Character &source, Character &target, int power, const StringInfo &stringInfo) const;
};

#endif