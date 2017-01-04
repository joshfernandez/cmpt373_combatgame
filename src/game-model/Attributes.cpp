#include "../../include/game/Attributes.hpp"
#include <algorithm>

Attributes::Attributes() {
    attributes = {
            {"strength", SECONDARY_BASE_NUM},
            {"intelligence", SECONDARY_BASE_NUM},
            {"dexterity", SECONDARY_BASE_NUM},
            {"vitality", VITALITY_BASE_NUM}
    };
}

std::string Attributes::getString() const {
    std::string attributesString = "";

    for (const auto& attribute : attributes) {
        attributesString += attribute.first + ": " + std::to_string(attribute.second) + "\n";
    }

    return attributesString;
}

int Attributes::getAttribute(const std::string &attribute) {
    return attributes[toLower(attribute)];
}

void Attributes::setPrimaryAttribute(const std::string &primaryAttribute) {
    if (isValidAttribute(primaryAttribute)) {
        attributes[toLower(primaryAttribute)] = PRIMARY_BASE_NUM;
    }
}

void Attributes::increaseAttribute(const std::string &attribute) {
    if (isValidAttribute(attribute)) {
        attributes[toLower(attribute)]++;
    }
}

bool Attributes::isValidAttribute(const std::string &attribute) {
    return attributes.find(toLower(attribute)) != attributes.end();
}

std::string Attributes::toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
