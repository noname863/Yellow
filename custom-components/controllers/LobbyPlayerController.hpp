//
// Created by Олег Бобров on 20.01.2020.
//

#ifndef WG_LOBBYPLAYERCONTROLLER_HPP
#define WG_LOBBYPLAYERCONTROLLER_HPP


#include "../../core/GameObject.h"
#include "../../network/Network.hpp"
#include "../Game.hpp"

class LobbyPlayerController : public Component {
private:
    bool isGameRunning(const nlohmann::json & json);
public:
    void start();
    void onDestroy();

    void onLogin(const nlohmann::json & json);
    void onMapLayer0(const nlohmann::json & json);
    void onMapLayer1(const nlohmann::json & json);
    void onMapLayer10(const nlohmann::json & json);
    void onGames(const nlohmann::json & json);
};


#endif //WG_LOBBYPLAYERCONTROLLER_HPP
