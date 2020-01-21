//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_CAMERACONTROLLER_H
#define WG_CAMERACONTROLLER_H


#include <SFML/Window/Keyboard.hpp>
#include "../../core/Component.h"

class CameraController : public Component {
public:
public:
    sf::Keyboard::Key up = sf::Keyboard::Up;
    sf::Keyboard::Key left = sf::Keyboard::Left;
    sf::Keyboard::Key down = sf::Keyboard::Down;
    sf::Keyboard::Key right = sf::Keyboard::Right;
    float maxSize = 1000;
    float minSize = 1;
    float scrollSpeed = 5.0f;
    float scrollFactor = 0.1f;
    float moveFactor = 0.3f;
    float targetWidth;
    Vector2 cursorScreenPosition;
    Vector2 targetTranslation;

    void start();
    void update();
};


#endif //WG_CAMERACONTROLLER_H
