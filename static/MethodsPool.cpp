#include "MethodsPool.hpp"
#include "../core/Component.h"
#include "../core/GameObject.h"

std::vector<UpdateWrapper> MethodsPool::updatePool;
std::vector<FixedUpdateWrapper> MethodsPool::fixedUpdatePool;
std::list<StartWrapper> MethodsPool::startPool;
std::list<GameObject *> MethodsPool::destroyObjectPool;
std::list<Component *> MethodsPool::destroyComponentPool;

void MethodsPool::removeFromUpdate(size_t index) {
    std::swap(updatePool[index], updatePool.back());
    reinterpret_cast<Component *>(updatePool[index].getObject())->updatePosition = index;
    MethodsPool::updatePool.pop_back();
}

void MethodsPool::removeFromFixedUpdate(size_t index) {
    std::swap(fixedUpdatePool[index], fixedUpdatePool.back());
    reinterpret_cast<Component *>(fixedUpdatePool[index].getObject())->fixedUpdatePosition = index;
    MethodsPool::fixedUpdatePool.pop_back();
}

void MethodsPool::start() {
    for (StartWrapper & start : MethodsPool::startPool) {
        start();
    }
    startPool.clear();
}

void MethodsPool::update() {

    // using indexing because vector size can change, and vector may resize
    size_t oldSize = MethodsPool::updatePool.size();
    for (size_t i = 0; i < oldSize; ++i) {
        MethodsPool::updatePool[i]();
    }
}

void MethodsPool::fixedUpdate() {
    size_t oldSize = MethodsPool::fixedUpdatePool.size();
    for (size_t i = 0; i < oldSize; ++i) {
        MethodsPool::fixedUpdatePool[i]();
    }
}

void MethodsPool::onDestroy() {
    for (Component * component : destroyComponentPool) {
        component->destroyImmediate();
    }
    destroyComponentPool.clear();
    for (GameObject * gameObject : destroyObjectPool) {
        gameObject->destroyImmediate();
    }
    destroyObjectPool.clear();
}
