//
// Created by Олег Бобров on 19.01.2020.
//

#ifndef WG_POINTPHYSICS_HPP
#define WG_POINTPHYSICS_HPP

#include "../../core/GameObject.h"
#include "../Point.h"

class PointPhysics : public Component {
private:
    Point * point;
    std::unordered_map<int, Point *>::iterator begin;
public:
    void start();
    void fixedUpdate();
    void onDestroy();
    void onMapLayer10(const nlohmann::json & json);
};


#endif //WG_POINTPHYSICS_HPP
