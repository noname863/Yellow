#include "PlayerConfig.hpp"
#include <fstream>
#include "../json.hpp"

nlohmann::json PlayerConfig::login;
std::string PlayerConfig::playerName;
std::string PlayerConfig::hostName;
size_t PlayerConfig::numPlayers;
size_t PlayerConfig::numTurns;

PlayerConfig::PlayerConfig() {
    std::ifstream configFile("playerConfig.json");
    configFile >> login;
    if (login.contains("name")) {
        PlayerConfig::playerName = login["name"];
    }
    if (login.contains("game")) {
        PlayerConfig::hostName = login["game"];
    }
    if (login.contains("num_players")) {
        PlayerConfig::numPlayers = login["num_players"];
    }
    if (login.contains("num_turns")) {
        PlayerConfig::numTurns = login["num_turns"];
    }

}

PlayerConfig PlayerConfig::playerConfigInit;
