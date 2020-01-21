//
// Created by Олег Бобров on 17.11.2019.
//

#include "CameraController.h"
#include "../../static/Time.h"
#include "../../static/Input.hpp"
#include "../../core-components/Camera.h"
#include "../../utility/Mathf.hpp"
#include <iostream>
#include <cmath>

void CameraController::start() {
    this->targetWidth = this->maxSize;
    Camera::mainCamera->setWidth(this->maxSize);
}

void CameraController::update() {
    Camera * mainCamera = Camera::mainCamera;
    Vector2 pos = this->transform->getPosition();
    if (Input::isKey(up)) {
        pos += {0.f, mainCamera->getWidth()*moveFactor*Time::deltaTime};
    }
    if (Input::isKey(left)) {
        pos -= {mainCamera->getWidth()*moveFactor*Time::deltaTime,0.f };
    }
    if (Input::isKey(down)) {
        pos -= {0.f, mainCamera->getWidth()*moveFactor*Time::deltaTime};
    }
    if (Input::isKey(right)) {
        pos += {mainCamera->getWidth()*moveFactor*Time::deltaTime,0.f };
    }
    if (Input::wheelScrolled) {
        this->targetWidth -=
                this->targetWidth *
                this->scrollFactor *
                Input::wheelScrollEvent.delta;
        this->targetWidth = Mathf::clamp(this->targetWidth, this->minSize, this->maxSize);
        this->cursorScreenPosition = Input::mousePosition;
        this->targetTranslation = mainCamera->screenToWorldPoint(this->cursorScreenPosition) - pos;
    }
    float width = mainCamera->getWidth();
    if (fabs(this->targetWidth - width) > 1e-2) {
        mainCamera->setWidth(
                Mathf::lerp(width, this->targetWidth, this->scrollSpeed*Time::deltaTime));
        Vector2 currentTranslation = mainCamera->screenToWorldPoint(this->cursorScreenPosition) - pos;
        pos += this->targetTranslation - currentTranslation;
        this->targetTranslation = currentTranslation;
//        Vector2 a = this->targetTranslation;
//        std::cout << a.x << " " << a.y <<  " ";
//        Vector2 b = currentTranslation;
//        std::cout << b.x << " " << b.y << std::endl;
    }
    this->transform->setPosition(pos);
}
