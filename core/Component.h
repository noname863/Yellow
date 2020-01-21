//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_COMPONENT_H
#define WG_COMPONENT_H

#include "Transform.h"
#include "../utility/methodWrapper.hpp"
#include <vector>
#include <limits>
#include <forward_list>

class Component {
    std::list<Component *>::iterator componentPosition;
    std::list<OnDestroyWrapper>::iterator destroyPosition;
    
    size_t updatePosition;
    size_t fixedUpdatePosition;
    
    void destroyImmediate();
    friend class GameObject;
    friend class MethodsPool;
public:
    GameObject *gameObject = nullptr; //TODO: prevent changes of this field
    Transform *transform = nullptr;

    //TODO: add add/getComponent functions
//    template <class T, class... Args>
//    T * addComponent(Args &&... args) {
//        gameObject->addComponent(std::forward<Args>(args)...);
//    }
//
//    template <class T>
//    T * getComponent() {
//        gameObject->addComponent(std::forward<Args>(args)...);
//    }
    void destroy();
    
    virtual ~Component() = default;
};


#endif //WG_COMPONENT_H
