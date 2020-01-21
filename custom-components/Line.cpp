//
// Created by Олег Бобров on 15.11.2019.
//

#include <iostream>
#include "Line.h"
#include "../static/Database.h"
#include "../core/GameObject.h"
#include "../utility/Mathf.hpp"

Line::Line(int idx) : Behaviour(idx) {

}

void Line::applyLayer0(const nlohmann::json &json) {
    this->length = json.value("length", this->length);
    if (json.contains("points")) {
        auto &item_points = json["points"];
        //TODO: check null array
        this->points[0] = Database::points[item_points[0]];
        this->points[1] = Database::points[item_points[1]];
        this->transform->setParent(this->points[0]->transform);
        this->points[0]->adjacent.emplace_back(this->points[1], this);
        this->points[1]->adjacent.emplace_back(this->points[0], this);
    }
}

void Line::start() {
    this->lineRenderer = this->gameObject->getComponent<LineRenderer>();
    this->lineRenderer->setVertices({0, 0}, {this->length, 0.f});
}

void Line::update() {
    Vector2 direction =
            this->points[1]->transform->getPosition() -
            this->points[0]->transform->getPosition();
    this->transform->setRotation(Mathf::RAD2DEG * atan2f(direction.y, direction.x));
    this->transform->setLocalScale(Vector2(direction.magnitude()/this->length));
}



