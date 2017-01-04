#include "game/Spell.hpp"

Spell::Spell() : Spell("", 0, SpellType::UNDEFINED, "") {}

Spell::Spell(const std::string& name, int manaCost, SpellType type, const std::string& effect)
		: name(name), manaCost(manaCost), type(type), effect(effect) {}

Spell::Spell(const std::string& name, int manaCost, SpellType type, const std::string& effect,
			 const std::string& hitchar, const std::string& hitroom, const std::string& hitvict)
        : name(name), manaCost(manaCost), type(type), effect(effect), hitchar(hitchar), hitroom(hitroom), hitvict(hitvict) {}

std::string Spell::getName() const {
    return this->name;
}

int Spell::getManaCost() const {
    return this->manaCost;
}

SpellType Spell::getType() const {
    return this->type;
}

int Spell::getPower(const Character &caster) const {
	if (effect != "") {
		return ExprEvaluator::evaluate_infix_1(effect, caster.getLevel());
	}
	else {
		return 0;
	}
}

const std::string& Spell::getHitchar() const {
	return hitchar;
};

const std::string& Spell::getHitroom() const {
	return hitroom;
};

const std::string& Spell::getHitvict() const {
	return hitvict;
};
