//
// Created by Олег Бобров on 19.11.2019.
//

#ifndef WG_GRAPHCONTROLLER_H
#define WG_GRAPHCONTROLLER_H


#include "../core/Component.h"
#include "../utility/json.hpp"
#include "../utility/GraphVisualizer.h"

class GraphController : public Component {
private:
    GraphVisualizer graphVisualizer;
    nlohmann::json playerInfo;
    bool is_mapInit = false;
public:
    std::unordered_map<int, std::list<int>> graph;

    void applyLayer0(const nlohmann::json& json);
    void applyLayer1(const nlohmann::json& json);
    void applyLayer10(const nlohmann::json& json);
    void applyForceMethod();
    void applyForceMethodIteration();

    void start();
    void update();
    void onDestroy();
    void onLogin(const nlohmann::json & json);
    void onMapLayer0(const nlohmann::json & json);
    void onMapLayer1(const nlohmann::json & json);
};


#endif //WG_GRAPHCONTROLLER_H
