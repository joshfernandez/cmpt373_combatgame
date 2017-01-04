#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <unordered_map>

class Attributes {
private:
    std::unordered_map<std::string, int> attributes;

public:
    const int PRIMARY_BASE_NUM = 10;
    const int SECONDARY_BASE_NUM = 9;
    const int VITALITY_BASE_NUM = 8;

    Attributes();
    std::string getString() const;
    int getAttribute(const std::string &attribute);
    void setPrimaryAttribute(const std::string &primaryAttribute);
    void increaseAttribute(const std::string &attribute);
    bool isValidAttribute(const std::string &attribute);
    std::string toLower(std::string s);
};

#endif
