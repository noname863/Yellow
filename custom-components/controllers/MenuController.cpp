//
// Created by Олег Бобров on 20.01.2020.
//

#include "MenuController.hpp"
#include "../../static/Prefabs.h"

const float buttonWidth = 200;
const float buttonHeight = 50;

void MenuController::start() {
    Network::onGamesResponse.addListener<MenuController, &MenuController::onGames>(this);
    Network::send(Action::GAMES);
    initHostButton();
}

void MenuController::update() {
    timeSinceRefresh += Time::deltaTime;
    if (timeSinceRefresh >= refreshTime) {
        timeSinceRefresh = 0.f;
        Network::send(Action::GAMES);
    }
}

void MenuController::onDestroy() {
    Network::onGamesResponse.removeListener<MenuController, &MenuController::onGames>(this);
    for (auto button : buttons) {
        button->gameObject->destroy();
    }
    hostButton->gameObject->destroy();
}

void MenuController::onGameSelected(int idx) {
    if (idx < 0) {
        PlayerConfig::login["game"] = PlayerConfig::hostName;
    } else {
        PlayerConfig::login["game"] = games[idx]["name"];
    }
    Game::current->setState(GameState::INIT);
}

void MenuController::onGames(const nlohmann::json & json) {
    //std::cout << json.dump(4) << std::endl;
    games = json["games"];
    filterGames();
    initButtons();
}

void MenuController::filterGames() {
    std::cout << games.dump(4) << std::endl;
    for (auto iter = games.cbegin(); iter != games.cend();) {
        if ((*iter)["state"] != GameState::INIT || (*iter)["num_players"] <= 1) {
            iter = games.erase(iter);
        }
        else {
            ++iter;
        }
    }
    std::cout << games.dump(4) << std::endl;
}

void MenuController::initButtons() {
    for (auto button : buttons) {
        button->gameObject->destroy();
    }
    int buttonsCount = games.size();
    buttons.resize(buttonsCount);
    for (int i = 0; i < buttonsCount; i++) {
        buttons[i] = Prefabs::button(buttonWidth, buttonHeight, games[i]["name"]);
        buttons[i]->buttonIdx = i;
        buttons[i]->onClick.addListener<MenuController, &MenuController::onGameSelected>(this);
        buttons[i]->transform->setLocalPosition({0, -(buttonHeight+1)*i});
        buttons[i]->gameObject->instantiate(transform);
    }
}

void MenuController::initHostButton() {
    hostButton = Prefabs::button(buttonWidth, buttonHeight, PlayerConfig::hostName);
    hostButton->buttonIdx = -1;
    hostButton->buttonColor = InterfaceConfig::playerTrainColor;
    hostButton->onClick.addListener<MenuController, &MenuController::onGameSelected>(this);
    hostButton->transform->setLocalPosition({-(buttonWidth+1), 0});
    hostButton->gameObject->instantiate(transform);
}
