//
// Created by Олег Бобров on 19.01.2020.
//

#include "PointPhysics.hpp"
#include "../../network/Network.hpp"
#include "../../static/Database.h"

void PointPhysics::start() {
    if (!point) {
        point = gameObject->getComponent<Point>();
    }
    begin = std::next(Database::points.find(point->idx));
    Network::onMapResponse10.addListener<PointPhysics, &PointPhysics::onMapLayer10>(this);
}

void PointPhysics::fixedUpdate() {
    //main forces
    for (auto it = begin; it != Database::points.end(); it++) {
        const auto & anotherPoint = it->second;
        Vector2 direction = anotherPoint->transform->getPosition() - point->transform->getPosition();
        float k = direction.magnitude() != 0.f ? ForceMethodConfig::charge / direction.magnitude() : 0.f;
        //float k = direction.sqrMagnitude() != 0.f ? ForceMethodConfig::charge * ForceMethodConfig::charge / direction.sqrMagnitude() : 0.f;
        anotherPoint->rigidBody->addForce(direction.normalized() * k);
        point->rigidBody->addForce(-direction.normalized() * k);
    }
    //drag forces
    Vector2 direction = -point->rigidBody->velocity;
    point->rigidBody->addForce(
            direction.normalized() *
            (ForceMethodConfig::frictionK * direction.sqrMagnitude() +
            ForceMethodConfig::dampK * sqrtf(direction.magnitude())));
    //rigidBody->addForce(direction.normalized() *(frictionK * direction.sqrMagnitude()));
}

void PointPhysics::onDestroy() {
    point->rigidBody->destroy();
    point->rigidBody = nullptr;
    Network::onMapResponse10.removeListener<PointPhysics, &PointPhysics::onMapLayer10>(this);
}

void PointPhysics::onMapLayer10(const nlohmann::json &json) {
    destroy();
}