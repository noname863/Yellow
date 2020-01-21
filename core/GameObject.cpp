//
// Created by Олег Бобров on 17.11.2019.
//

#include "GameObject.h"

//void GameObject::update() {
//    for (UpdateWrapper & method : updatePool) {
//        method();
//    }
//    for (Transform * child : *transform) {
//        child->gameObject->update();
//    }
//}

GameObject * GameObject::instantiate() {
    for (StartWrapper & method : startPool) {
        MethodsPool::startPool.push_back(method);
    }
    this->startPool.clear();
    for (UpdateWrapper & method : updatePool) {
        reinterpret_cast<Component *>(method.getObject())->updatePosition = 
                MethodsPool::updatePool.size();
        MethodsPool::updatePool.push_back(method);
    }
    this->updatePool.clear();
    for (FixedUpdateWrapper & method : fixedUpdatePool) {
        reinterpret_cast<Component *>(method.getObject())->fixedUpdatePosition = 
                MethodsPool::fixedUpdatePool.size();
        MethodsPool::fixedUpdatePool.push_back(method);
    }
    this->fixedUpdatePool.clear();
    onScene = true;
    return this;
}

GameObject * GameObject::instantiate(Transform * parent) {
    GameObject::instantiate();
    GameObject::transform->setParent(parent);
    return this;
}


void GameObject::destroyImmediate() {
    if (transform->parent) {
        transform->parent->children.erase(transform->it);
    }
    if (this->onScene) {
        sceneDestroy();
    }
    delete this;
}

void GameObject::destroy() {
    MethodsPool::destroyObjectPool.push_back(this);
}

void GameObject::sceneDestroy() {
    while (!components.empty()) {
        components.front()->destroyImmediate();
    }
    for (Transform * child : *transform) {
        child->gameObject->sceneDestroy();
    }
}

GameObject::GameObject() {
    transform = new Transform(this);
}

GameObject::~GameObject() {
    for (Component * component : components) {
        delete component;
    }
    delete transform;
}

