#ifndef PLAYERCONFIG_HPP
#define PLAYERCONFIG_HPP

#include <string>
#include "../json.hpp"

class PlayerConfig
{
public:
    PlayerConfig();
    static nlohmann::json login;
    static std::string playerName;
    static std::string hostName;
    static size_t numPlayers;
    static size_t numTurns;
    // trick to init config from file
    static PlayerConfig playerConfigInit;
};

#endif // PLAYERCONFIG_HPP
