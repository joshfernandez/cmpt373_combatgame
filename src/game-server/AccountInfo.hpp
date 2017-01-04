#ifndef ADVENTURE2016_ACCOUNTINFO_HPP
#define ADVENTURE2016_ACCOUNTINFO_HPP

#include "PlayerRole.hpp"
#include <string>

struct AccountInfo {
    std::string username;
    int playerRoleFlags;

    bool hasRole(PlayerRole role) const {
        return (bool)(playerRoleFlags & (int)role);
    }

    bool operator==(const AccountInfo& other) const {
        return username == other.username;
    }
};

struct AccountInfoHash {
    size_t operator()(const AccountInfo& other) const
    {
        return std::hash<std::string>()(other.username);
    }
};

#endif //ADVENTURE2016_ACCOUNTINFO_HPP
