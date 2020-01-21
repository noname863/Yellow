//
// Created by Олег Бобров on 10.01.2020.
//

#ifndef WG_CIRCLECOLLIDER_HPP
#define WG_CIRCLECOLLIDER_HPP


#include "Collider.hpp"

class CircleCollider : public Collider {
private:
    float radius;
public:
    float getRadius() const;

    void setRadius(float radius);

    bool isOverlapsPoint(const Vector2 & point) const override;
};


#endif //WG_CIRCLECOLLIDER_HPP
