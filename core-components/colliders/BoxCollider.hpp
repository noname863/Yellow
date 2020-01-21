//
// Created by Олег Бобров on 10.01.2020.
//

#ifndef WG_BOXCOLLIDER_HPP
#define WG_BOXCOLLIDER_HPP

#include <utility>
#include "../../linalg/Vector2.hpp"
#include "Collider.hpp"

class BoxCollider : public Collider {
private:
    float width;
    float height;
public:
    void setWidthHeight(float width, float height);
    
    std::pair<float, float> getWidthHeight() const;

    bool isOverlapsPoint(const Vector2 & point) const override;
};


#endif //WG_BOXCOLLIDER_HPP
