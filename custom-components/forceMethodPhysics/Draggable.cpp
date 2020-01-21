//
// Created by Олег Бобров on 19.01.2020.
//

#include "Draggable.hpp"
#include "PointPhysics.hpp"
#include "LinePhysics.hpp"
#include "../../network/Network.hpp"

void Draggable::start() {
    Network::onMapResponse10.addListener<Draggable, &Draggable::onMapLayer10>(this);
    collider = gameObject->getComponent<Collider>();
    rigidBody = gameObject->getComponent<RigidBody>();
}

void Draggable::update() {
    // Vector2 screenPoint = Input::mousePosition;
    Vector2 worldPoint = Camera::mainCamera->screenToWorldPoint(Input::mousePosition);

    if (Input::getMouseButtonPressed(sf::Mouse::Button::Left)) {
        if (collider->isOverlapsPoint(worldPoint)) {
            isDragging = true;
            rigidBody->setKinematic(true);
        }
    }
    if (isDragging) {
        transform->setPosition(worldPoint);
    }
    if (Input::getMouseButtonReleased(sf::Mouse::Button::Left) && isDragging) {
        isDragging = false;
        rigidBody->setKinematic(false);
    }
}

void Draggable::onDestroy() {
    Network::onMapResponse10.removeListener<Draggable, &Draggable::onMapLayer10>(this);
}

void Draggable::onMapLayer10(const nlohmann::json &json) {
    destroy();
}
