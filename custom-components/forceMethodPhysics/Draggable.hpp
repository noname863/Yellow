//
// Created by Олег Бобров on 19.01.2020.
//

#ifndef WG_DRAGGABLE_HPP
#define WG_DRAGGABLE_HPP

#include "../../core/GameObject.h"
#include "../../core-components/colliders/Collider.hpp"
#include "../../core-components/Camera.h"
#include "../../static/Input.hpp"
#include "../../core-components/RigidBody.hpp"
#include "../../static/Database.h"
#include "../../utility/Config.hpp"

class Draggable : public Component {
private:
    bool isDragging = false;
    Collider * collider = nullptr;
    RigidBody * rigidBody = nullptr;
public:
    void start();
    void update();
    void onDestroy();
    void onMapLayer10(const nlohmann::json & json);
};


#endif //WG_DRAGGABLE_HPP
