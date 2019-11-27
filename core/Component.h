//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_COMPONENT_H
#define WG_COMPONENT_H

#include "Transform.h"

class Component {
public:
    GameObject *gameObject = nullptr; //TODO: prevent changes of this field
    Transform *transform = nullptr;

    virtual ~Component() = default;

    virtual void update();
    virtual void onDraw();//remove this
    //TODO: add start() event (check add to scene and call)
};


#endif //WG_COMPONENT_H