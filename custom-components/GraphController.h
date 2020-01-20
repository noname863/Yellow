//
// Created by Олег Бобров on 19.11.2019.
//

#ifndef WG_GRAPHCONTROLLER_H
#define WG_GRAPHCONTROLLER_H


#include "../core/Component.h"
#include "../utility/json.hpp"
#include "../utility/GraphVisualizer.h"
#include "PlayerController.hpp"
#include "../linalg/Vector2.hpp"
#include "../utility/Config.hpp"
#include "../core-components/colliders/Collider.hpp"

class GraphController : public Component {
private:
    float graphSize = 0.f;
    PlayerController * playerController;
    nlohmann::json playerInfo;
    nlohmann::json layer0;
    nlohmann::json layer1;
public:
    void applyLayer0(const nlohmann::json & json);
    void applyLayer1(const nlohmann::json & json);
    void applyLayer10(const nlohmann::json & json);
    void applyPlayerInfo(const nlohmann::json & json);
    bool isGameRunning(const nlohmann::json & json);

    void start();
    void onDestroy();
    void onLogin(const nlohmann::json & json);
    void onMapLayer0(const nlohmann::json & json);
    void onMapLayer1(const nlohmann::json & json);
    void onMapLayer10(const nlohmann::json & json);
    void onPlayer(const nlohmann::json & json);
    void onGames(const nlohmann::json & json);
};


#endif //WG_GRAPHCONTROLLER_H
