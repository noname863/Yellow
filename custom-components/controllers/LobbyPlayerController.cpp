//
// Created by Олег Бобров on 20.01.2020.
//

#include <iostream>
#include "LobbyPlayerController.hpp"
#include "../../utility/Config.hpp"

void LobbyPlayerController::start() {
    Network::onLoginResponse.addListener<LobbyPlayerController, &LobbyPlayerController::onLogin>(this);
    Network::onMapResponse0.addListener<LobbyPlayerController, &LobbyPlayerController::onMapLayer0>(this);
    Network::onMapResponse1.addListener<LobbyPlayerController, &LobbyPlayerController::onMapLayer1>(this);
    Network::onMapResponse10.addListener<LobbyPlayerController, &LobbyPlayerController::onMapLayer10>(this);
    Network::onGamesResponse.addListener<LobbyPlayerController, &LobbyPlayerController::onGames>(this);

    Network::send(Action::LOGIN, PlayerConfig::login);
    Network::send(Action::MAP, {{"layer", 0}});
    Network::send(Action::MAP, {{"layer", 1}});
    //Network::send(Action::MAP, {{"layer", 10}});
    Network::send(Action::GAMES);
}

void LobbyPlayerController::onDestroy() {
    Network::onLoginResponse.removeListener<LobbyPlayerController, &LobbyPlayerController::onLogin>(this);
    Network::onMapResponse0.removeListener<LobbyPlayerController, &LobbyPlayerController::onMapLayer0>(this);
    Network::onMapResponse1.removeListener<LobbyPlayerController, &LobbyPlayerController::onMapLayer1>(this);
    Network::onMapResponse10.removeListener<LobbyPlayerController, &LobbyPlayerController::onMapLayer10>(this);
    Network::onGamesResponse.removeListener<LobbyPlayerController, &LobbyPlayerController::onGames>(this);
}

void LobbyPlayerController::onLogin(const nlohmann::json & json) {
    Game::current->trySetPlayerInfo(json);
}

void LobbyPlayerController::onMapLayer0(const nlohmann::json & json) {
    Game::current->trySetLayer0(json);
    Game::current->applyLayer0(json);
}

void LobbyPlayerController::onMapLayer1(const nlohmann::json & json) {
    Game::current->trySetLayer1(json);
    Game::current->applyLayer1(json);
    Game::current->applyPlayerInfo(json);
}

void LobbyPlayerController::onMapLayer10(const nlohmann::json & json) {
    Game::current->trySetLayer10(json);
    Game::current->applyLayer10(json);
}

void LobbyPlayerController::onGames(const nlohmann::json & json) {
    if (isGameRunning(json)) {
        Game::current->setState(GameState::RUN);
    }
    else {
        Network::send(Action::GAMES);
    }
}

bool LobbyPlayerController::isGameRunning(const nlohmann::json & json) {
    //std::cout << json.dump(4) << std::endl;
    if (json.contains("games")) {
        for (auto game : json["games"]) {
            if (game["name"] == PlayerConfig::login["game"] && game["state"] == GameState::RUN) {
                return true;
            }
        }
    }
    return false;
}
