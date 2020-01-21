#include "Button.hpp"
#include "../../static/Input.hpp"
#include "../../utility/Config.hpp"
#include "../Camera.h"
#include <iostream>

sf::Color multipleColorByValue(const sf::Color & color, float val) {
    return {static_cast<sf::Uint8>(static_cast<float>(color.r) * val), 
            static_cast<sf::Uint8>(static_cast<float>(color.g) * val), 
            static_cast<sf::Uint8>(static_cast<float>(color.b) * val)};
}

void Button::update() {
    Vector2 worldPoint = Camera::mainCamera->screenToWorldPoint(Input::mousePosition);    
    bool overlaps = this->buttonCollider->isOverlapsPoint(worldPoint);
    if (overlaps) {
        if (Input::getMouseButtonHeld(sf::Mouse::Left)) {
            renderer->rectangle.setFillColor(
                multipleColorByValue(buttonColor,
                                     InterfaceConfig::buttonDarkFloat * 
                                     InterfaceConfig::buttonDarkFloat));
        } else if (Input::getMouseButtonReleased(sf::Mouse::Left)) {
            renderer->rectangle.setFillColor(buttonColor);
            onClick.invoke(buttonIdx);
        } else {
            renderer->rectangle.setFillColor(
                multipleColorByValue(buttonColor,
                                     InterfaceConfig::buttonDarkFloat));
        }
    } else {
        renderer->rectangle.setFillColor(buttonColor);
    }
}

