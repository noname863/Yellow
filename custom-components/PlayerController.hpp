//
// Created by Олег Бобров on 10.12.2019.
//

#ifndef WG_PLAYERCONTROLLER_HPP
#define WG_PLAYERCONTROLLER_HPP


#include "../core/Component.h"
#include "Point.h"
#include <bitset>


class El {
//TODO: remove this structure or make private
public:
    std::vector<int> way;
    std::bitset<2000> used;

    int value = 0;
    int spent = 0;
    int vertex = 0;
    int maxValue = 0;
    int startVertex = 0;

    int marketPos = 0;
    int position = 0;
    Line* line = 0;

};


class PlayerController : public Component {
private:
    float timeFromLastTurn = 0.5f;
    float waitingTime = 0.5f;//time to wait (show animations)
public:

    int tickNow = 0;   /// must have

    Point * playerPoint = nullptr;
    Town * playerTown = nullptr;
    std::vector<Train*> playerTrains;
    std::vector<Market*> markets;
    std::vector<Storage*> storages;
    bool isMapUpdated = false;

    void update();
    void trainUpgrade();
    void tryTrainUpdate(Train*);
    void strategyIteration();
    std::pair<std::vector<int>, int> trainWayToProducts(Train*);
    std::pair<std::vector<int>, int> trainWayToStorage(Train*);

    void addTimeTable(std::vector<int> );
    bool isFreeTimeTable(El );
    bool isTown(int );

    void clearVertex();

    std::map<int, int> canGo;


};



#endif //WG_PLAYERCONTROLLER_HPP
