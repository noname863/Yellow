//
// Created by Олег Бобров on 15.11.2019.
//

#include <iostream>
#include "Point.h"
#include "../static/Database.h"
#include "../core/GameObject.h"
#include "../linalg/Vector2.hpp"

void Point::applyLayer10(const nlohmann::json &json) {
    this->transform->setLocalPosition({
        json.value("x", 0.f),
        json.value("y", 0.f) });
}

void Point::applyLayer0(const nlohmann::json &json) {
    this->adjacent.clear();
    if (json.contains("post_idx")) {
        this->post = json["post_idx"].is_null() ? nullptr : Database::posts[json["post_idx"]];
    }
}

Point::Point(int idx) : Behaviour(idx) {

}

void Point::start() {
    if (!rigidBody) {
        this->rigidBody = this->gameObject->getComponent<RigidBody>();
    }
}
