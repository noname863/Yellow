//
// Created by Олег Бобров on 10.12.2019.
//

#ifndef WG_PLAYERCONTROLLER_HPP
#define WG_PLAYERCONTROLLER_HPP


#include "../../core/Component.h"
#include "../Point.h"
#include <bitset>


class El {
//TODO: remove this structure or make private
public:
    std::vector<int> way;

    int value = 0;
    int spent = 0;
    int vertex = 0;
    int maxValue = 0;

    int marketPos = 0;
    int position = 0;

    Line* line = nullptr;
    Point* point = nullptr;
};


class PlayerController : public Component {
private:
public:
    void start();
    void update();
    void onDestroy();
    void trainUpgrade();
    void tryTrainUpdate(Train*);
    void strategyIteration();
    std::pair<std::vector<int>, int> trainWayToProducts(Train*);
    std::pair<std::vector<int>, int> trainWayToStorage(Train*);

    void addTimeTable(std::vector<int>, int);
    bool isFreeTimeTable(El );
    bool isTown(int );
    void trainIteration(Train*);

    std::map<int, int> canGo;

    bool isGameFinished(const nlohmann::json & json);

    void onMapLayer1(const nlohmann::json & json);
    void onGames(const nlohmann::json & json);
};



#endif //WG_PLAYERCONTROLLER_HPP
