//
// Created by Олег Бобров on 20.01.2020.
//

#include "Game.hpp"
#include "../static/Database.h"
#include "../static/Prefabs.h"
#include "../utility/random.hpp"
#include "controllers/CameraController.h"
#include "controllers/MenuController.hpp"
#include "controllers/PlayerController.hpp"
#include "controllers/LobbyPlayerController.hpp"
#include "../core-components/renderers/CircleRenderer.h"

using Random = effolkronium::random_static;

Game * Game::current = nullptr;

Game::Game() {
    //TODO: singleton
    if (!Game::current) {
        Game::current = this;
    }
}

void Game::start() {
    Network::connect("wgforge-srv.wargaming.net", 443);
    setState(Game::state);
}

void Game::onDestroy() {
    Network::disconnect();
}

GameState Game::getState() {
    return this->state;
}

void Game::setState(GameState state) {
    switch (state) {
        case GameState::MENU:
            Database::reset();
            layer0 = layer1 = layer10 = playerInfo = nlohmann::json();
            if (playerController) {
                playerController->destroy();
                playerController = nullptr;
            }
            if (lobbyPlayerController) {
                lobbyPlayerController->destroy();
                lobbyPlayerController = nullptr;
            }
            menuController = gameObject->addComponent<MenuController>();
            break;
        case GameState::INIT:
            if (menuController) {
                menuController->destroy();
                menuController = nullptr;
            }
            if (playerController) {
                playerController->destroy();
                playerController = nullptr;
            }
            lobbyPlayerController = gameObject->addComponent<LobbyPlayerController>();
            break;
        case GameState::RUN:
            if (menuController) {
                menuController->destroy();
                menuController = nullptr;
            }
            if (lobbyPlayerController) {
                lobbyPlayerController->destroy();
                lobbyPlayerController = nullptr;
            }
            playerController = gameObject->addComponent<PlayerController>();
            break;
        case GameState::FINISHED:
            //TODO: decide what to do when game finished
            if (menuController) {
                menuController->destroy();
                menuController = nullptr;
            }
            if (playerController) {
                playerController->destroy();
                playerController = nullptr;
            }
            if (lobbyPlayerController) {
                lobbyPlayerController->destroy();
                lobbyPlayerController = nullptr;
            }
            break;
        default:
            break;
    }
}

void Game::applyLayer0(const nlohmann::json &json) {
    graphSize = Camera::mainCamera->gameObject->getComponent<CameraController>()->maxSize;
    if (json.contains("points")) {
        for (const auto & item : json["points"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto &point = Database::points[idx];
                if (!point) {
                    point = Prefabs::point(idx);
                    point->gameObject->instantiate(this->transform);
                }
                point->applyLayer0(item);
                point->transform->setPosition({Random::get(-this->graphSize, this->graphSize),
                                               Random::get(-this->graphSize, this->graphSize)});
            }
        }
    }
    if (json.contains("lines")) {
        for (const auto &item : json["lines"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto &line = Database::lines[idx];
                if (!line) {
                    line = Prefabs::line(idx);
                    line->gameObject->instantiate(this->transform);
                }
                line->applyLayer0(item);
            }
        }
    }
}

void Game::applyLayer1(const nlohmann::json &json) {
    //TODO: find difference between jsons
    Database::isMapUpdated = true;
    Database::tick++;
    if (json.contains("posts")) {
        Database::markets.clear();
        Database::storages.clear();
        for (const auto &item : json["posts"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto & post = Database::posts[idx];
                PostType type = item.value("type", PostType::DEFAULT);
                switch (type) {
                    case PostType::TOWN: {
                        if (!post) {
                            post = Prefabs::town(idx);
                            post->gameObject->instantiate();
                        }
                        post->applyLayer1(item);
                        break;
                    }
                    case MARKET: {
                        if (!post) {
                            post = Prefabs::market(idx);
                            post->gameObject->instantiate();
                        }
                        Database::markets.push_back(static_cast<Market*>(post));
                        post->applyLayer1(item);
                        break;
                    }
                    case STORAGE: {
                        if (!post) {
                            post = Prefabs::storage(idx);
                            post->gameObject->instantiate();
                        }
                        Database::storages.push_back(static_cast<Storage*>(post));
                        post->applyLayer1(item);
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    }
    if (json.contains("trains")) {
        for (const auto & item : json["trains"]) {
            int idx = item.value("idx", -1);
            if (idx == -1) {
                continue;
            }
            auto & train = Database::trains[idx];
            if (!train) {
                train = Prefabs::train(idx);
                train->gameObject->instantiate();
            }
            train->applyLayer1(item);
        }
    }
}

void Game::applyLayer10(const nlohmann::json &json) {
    if (json.contains("coordinates")) {
        for (const auto &item : json["coordinates"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto &point = Database::points[idx];
                if (!point) {
                    point = Prefabs::point(idx);
                    point->gameObject->instantiate(this->transform);
                }
                point->applyLayer10(item);
            }
        }
    }
}

void Game::applyPlayerInfo(const nlohmann::json &json) {
    Database::playerPoint = Database::points[playerInfo["home"]["idx"]];
    Database::playerTown = static_cast<Town*>(Database::posts[playerInfo["home"]["post_idx"]]);
    Database::playerTown->gameObject->getComponent<CircleRenderer>()->circle.setFillColor(InterfaceConfig::playerTownColor);
    if (this->playerInfo.contains("trains")) {
        Database::playerTrains.clear();
        for (const auto & item : this->playerInfo["trains"]) {
            int idx = item.value("idx", -1);
            if (idx == -1) {
                continue;
            }
            auto & train = Database::trains[idx];
            if (train) {
                Database::playerTrains.push_back(train);
                train->gameObject->getComponent<CircleRenderer>()->circle.setFillColor(InterfaceConfig::playerTrainColor);
            }
        }
    }
}

bool Game::trySetLayer0(const nlohmann::json &json) {
    if (this->layer0 != json) {
        this->layer0 = json;
        return true;
    }
    return false;
}
bool Game::trySetLayer1(const nlohmann::json &json) {
    if (this->layer1 != json) {
        this->layer1 = json;
        return true;
    }
    return false;
}
bool Game::trySetLayer10(const nlohmann::json &json) {
    if (this->layer10 != json) {
        this->layer10 = json;
        return true;
    }
    return false;
}
bool Game::trySetPlayerInfo(const nlohmann::json &json) {
    if (this->playerInfo != json) {
        this->playerInfo = json;
        return true;
    }
    return false;
}
