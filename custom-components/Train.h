//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_TRAIN_H
#define WG_TRAIN_H

#include "Behaviour.h"
#include "../linalg/Vector2.hpp"

enum GoodsType {
    Nothing = 0,
    Products = 1,
    Armor = 2
};

class Train : public Behaviour {
private:
public:
    explicit Train(int idx);

    int cooldown = 0;
    int fuel = 0;
    int fuel_capacity = 0;
    int fuel_consumption = 0;
    int goods = 0;
    int goods_capacity = 0;
    int level = 0;
    int next_level_price = 0;
    int position = 0;
    int prevPosition = 0;
    int speed = 0;
    std::string player_idx;
    Line *line = nullptr;
    Line *prevLine = nullptr;
    GoodsType goods_type;

    int move_type = 0;
    int destiny = 0;
    std::vector<int> needWay;

    float animationSpeed = 10.0f;
    void applyLayer1(const nlohmann::json &json) override;
    void update();
};


#endif //WG_TRAIN_H
