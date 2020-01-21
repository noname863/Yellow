//
// Created by Олег Бобров on 20.01.2020.
//

#ifndef WG_GAME_HPP
#define WG_GAME_HPP


#include "../core/GameObject.h"
#include "../network/Network.hpp"

class MenuController;
class PlayerController;
class LobbyPlayerController;

class Game : public Component {
private:
    float graphSize = 0.f;
    nlohmann::json layer0;
    nlohmann::json layer1;
    nlohmann::json layer10;
    nlohmann::json playerInfo;

    GameState state = GameState::MENU;
    MenuController * menuController = nullptr;
    PlayerController * playerController = nullptr;
    LobbyPlayerController * lobbyPlayerController = nullptr;
public:
    static Game * current;
    Game();

    GameState getState();
    void setState(GameState state);

    void start();
    void onDestroy();

    bool trySetLayer0(const nlohmann::json & json);
    bool trySetLayer1(const nlohmann::json & json);
    bool trySetLayer10(const nlohmann::json & json);
    bool trySetPlayerInfo(const nlohmann::json & json);

    void applyLayer0(const nlohmann::json & json);
    void applyLayer1(const nlohmann::json & json);
    void applyLayer10(const nlohmann::json & json);
    void applyPlayerInfo(const nlohmann::json & json);
};


#endif //WG_GAME_HPP
