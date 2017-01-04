#ifndef SPELL_HPP
#define SPELL_HPP

#include <string>
#include <game/Character.hpp>
#include <expr-evaluator/ExprEvaluator.hpp>

enum class SpellType {
    OFFENSE,
    DEFENSE,
    BODY_SWAP,
	PIG_LATIN,
    UNDEFINED
};

class Spell {

public:

    Spell();
	Spell(const std::string& name, int manaCost, SpellType type, const std::string& effect);
    Spell(const std::string& name, int manaCost, SpellType type, const std::string& effect,
		  const std::string& hitchar, const std::string& hitroom, const std::string& hitvict);

    std::string getName() const;
    int getManaCost() const;
    int getPower() const;
    SpellType getType() const;
    int getPower(const Character& caster) const;
	const std::string& getHitchar() const;
	const std::string& getHitroom() const;
	const std::string& getHitvict() const;

private:

    std::string name;
    int manaCost;
    int power;
    std::string effect;
    SpellType type;
	std::string hitchar;
	std::string hitroom;
	std::string hitvict;

};

#endif