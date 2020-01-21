//
// Created by Олег Бобров on 10.01.2020.
//

#include "BoxCollider.hpp"
#include "../../core/GameObject.h"
#include <cmath>

void BoxCollider::setWidthHeight(float width, float height) {
    this->width = width;
    this->height = height;
}

std::pair<float, float> BoxCollider::getWidthHeight() const {
    return std::make_pair(width, height);
}

bool BoxCollider::isOverlapsPoint(const Vector2 & point) const {
    Vector2 localPoint = this->transform->toLocalPosition(point);
    // here comes strange way to check point
    return (std::abs(localPoint.x) < this->width / 2 && 
            std::abs(localPoint.y) < this->height / 2);
}
