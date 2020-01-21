//
// Created by Олег Бобров on 17.11.2019.
//

#include "GameObject.h"
#include "Component.h"
#include "../static/MethodsPool.hpp"


void Component::destroyImmediate() {
    if (destroyPosition != gameObject->onDestroyPool.end()) {
        (*destroyPosition)();
        gameObject->onDestroyPool.erase(destroyPosition);        
    }
    if (this->updatePosition != std::numeric_limits<size_t>::max()) {
        MethodsPool::removeFromUpdate(this->updatePosition);        
    }
    if (this->fixedUpdatePosition != std::numeric_limits<size_t>::max()) {
        MethodsPool::removeFromFixedUpdate(this->fixedUpdatePosition);
    }
    this->gameObject->components.erase(this->componentPosition);
    delete this;
}

void Component::destroy() {
    MethodsPool::destroyComponentPool.push_back(this);
}
