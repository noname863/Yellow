//
// Created by Олег Бобров on 19.01.2020.
//

#include "LinePhysics.hpp"
#include "../../network/Network.hpp"

void LinePhysics::start() {
    if (!line) {
        line = gameObject->getComponent<Line>();
    }
    Network::onMapResponse10.addListener<LinePhysics, &LinePhysics::onMapLayer10>(this);
}

void LinePhysics::fixedUpdate() {
    Vector2 direction =
            line->points[1]->transform->getPosition() -
            line->points[0]->transform->getPosition();
    float worldLength = direction.magnitude();
    float deltaLength = worldLength - ForceMethodConfig::springLength*line->length;
    direction.normalize();
    line->points[0]->rigidBody->addForce(
            direction * ForceMethodConfig::stiffnessK * deltaLength / line->length);
    line->points[1]->rigidBody->addForce(
            -direction * ForceMethodConfig::stiffnessK * deltaLength / line->length);
}

void LinePhysics::onDestroy() {
    Network::onMapResponse10.removeListener<LinePhysics, &LinePhysics::onMapLayer10>(this);
}

void LinePhysics::onMapLayer10(const nlohmann::json &json) {
    destroy();
}