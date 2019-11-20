//
// Created by Олег Бобров on 15.11.2019.
//

#include "Train.h"
#include "../static/Database.h"

Train::Train(int idx) : Behaviour(idx) {
    
}

void Train::applyLayer1(const nlohmann::json &json) {
    Train::cooldown = json["cooldown"];
    //Train::fuel = json["fuel"];
    //Train::fuel_capacity = json["fuel_capacity"];
    //Train::fuel_consumption = json["fuel_consumption"];
    Train::goods = json["goods"];
    Train::goods_capacity = json["goods_capacity"];
    //TODO: ask what this field store
    //"goods_type": null, WTF?
    Train::level = json["level"];
    Train::next_level_price = json["next_level_price"];
    //"player_idx": "a33dc107-04ab-4039-9578-1dccd00867d1",
    Train::position = json["position"];
    Train::speed = json["speed"];
    auto & line_idx = json["line_idx"];
    if (line_idx != nullptr) {
        Train::line = Database::lines[line_idx];
        Train::transform->setParent(Train::line->transform);
    } else {
        //TODO: alert that line_idx = null (exception or smth else)
    }
}
