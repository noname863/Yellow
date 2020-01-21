//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_GAMEOBJECT_H
#define WG_GAMEOBJECT_H


#include "Transform.h"
#include "Component.h"
#include <utility>
#include <cstdint>
#include <forward_list>
#include <string>
#include "../utility/methodWrapper.hpp"
#include "../static/MethodsPool.hpp"
#include "../utility/InterfaceChecker.hpp"

class GameObject {
private:
    std::list<Component*> components;
    std::forward_list<StartWrapper> startPool;
    std::forward_list<UpdateWrapper> updatePool;
    std::forward_list<FixedUpdateWrapper> fixedUpdatePool;
    std::list<OnDestroyWrapper> onDestroyPool;
    bool onScene = false;
    
    template <class T, class... Args>
    T * initComponent(Args &&... args);
    
    template <class T>
    typename std::enable_if<HasStart<T>::value>::type
    addStart(T * component);

    template <class T>
    typename std::enable_if<!HasStart<T>::value>::type
    addStart(T * ) {}
    
    template <class T>
    typename std::enable_if<HasUpdate<T>::value>::type
    addUpdate(T * component);
    
    template <class T>
    typename std::enable_if<!HasUpdate<T>::value>::type
    addUpdate(T * );
    
    template <class T>
    typename std::enable_if<HasFixedUpdate<T>::value>::type
    addFixedUpdate(T * component);
    
    template <class T>
    typename std::enable_if<!HasFixedUpdate<T>::value>::type
    addFixedUpdate(T * );
    
    template <class T>
    typename std::enable_if<HasOnDestroy<T>::value>::type
    addOnDestroy(T * component);
    
    template <class T>
    typename std::enable_if<!HasOnDestroy<T>::value>::type
    addOnDestroy(T * );

    template <class T>
    typename std::enable_if<HasStart<T>::value>::type
    onSceneStart(T * component);

    template <class T>
    typename std::enable_if<!HasStart<T>::value>::type
    onSceneStart(T * ) {}
    
    template <class T>
    typename std::enable_if<HasUpdate<T>::value>::type
    onSceneUpdate(T * component);
    
    template <class T>
    typename std::enable_if<!HasUpdate<T>::value>::type
    onSceneUpdate(T * );
    
    template <class T>
    typename std::enable_if<HasFixedUpdate<T>::value>::type
    onSceneFixedUpdate(T * component);
    
    template <class T>
    typename std::enable_if<!HasFixedUpdate<T>::value>::type
    onSceneFixedUpdate(T * );
    
    void sceneDestroy();
    
    friend class MethodsPool;
    friend class Component;
public:
    std::string name;
    Transform * transform = nullptr; //TODO: prevent changes of this field
    GameObject();
    ~GameObject();
    GameObject * instantiate();
    GameObject * instantiate(Transform * parent);

    template <class T, class... Args>
    T * addComponent(Args &&... args);
    
    template <class T>
    T * getComponent();
    
    void destroyImmediate();
    void destroy();
};

template <class T, class... Args>
T * GameObject::initComponent(Args &&... args) {
    T * instance = new T(std::forward<Args>(args)...);
    components.push_back(instance);
    instance->componentPosition = std::prev(components.end());
    instance->gameObject = this;
    instance->transform = GameObject::transform;
    return instance;
}

template <class T, class... Args>
T * GameObject::addComponent(Args &&... args) {
    T * instance = this->template initComponent<T>(std::forward<Args>(args)...);
    if (onScene) {
        onSceneStart(instance);
        onSceneUpdate(instance);
        onSceneFixedUpdate(instance);
    } else {
        addStart(instance);
        addUpdate(instance);
        addFixedUpdate(instance);
    }
    addOnDestroy(instance);
    return instance;
}

template<class T>
T * GameObject::getComponent() {
    T * result;
    for (Component * component : components) {
        result = dynamic_cast<T *>(component);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

template <class T>
typename std::enable_if<HasStart<T>::value>::type 
GameObject::addStart(T * component) {
    startPool.push_front(StartWrapper(component));
}

template <class T>
typename std::enable_if<HasUpdate<T>::value>::type 
GameObject::addUpdate(T * component) {
    updatePool.push_front(UpdateWrapper(component));
}

template <class T>
typename std::enable_if<!HasUpdate<T>::value>::type
GameObject::addUpdate(T * component) {
    component->updatePosition = std::numeric_limits<size_t>::max();
}

template <class T>
typename std::enable_if<HasFixedUpdate<T>::value>::type
GameObject::addFixedUpdate(T * component) {
    fixedUpdatePool.push_front(FixedUpdateWrapper(component));
}

template <class T>
typename std::enable_if<!HasFixedUpdate<T>::value>::type
GameObject::addFixedUpdate(T * component) {
    component->fixedUpdatePosition = std::numeric_limits<size_t>::max();
}

template <class T>
typename std::enable_if<HasOnDestroy<T>::value>::type
GameObject::addOnDestroy(T * component) {
    onDestroyPool.push_front(OnDestroyWrapper(component));
    component->destroyPosition = onDestroyPool.begin();
}

template <class T>
typename std::enable_if<!HasOnDestroy<T>::value>::type
GameObject::addOnDestroy(T * component) {
    component->destroyPosition = onDestroyPool.end();
}

template <class T>
typename std::enable_if<HasStart<T>::value>::type
GameObject::onSceneStart(T * component) {
    MethodsPool::startPool.push_back(StartWrapper(component));
}

template <class T>
typename std::enable_if<HasUpdate<T>::value>::type
GameObject::onSceneUpdate(T * component) {
    component->updatePosition = MethodsPool::updatePool.size();    
    MethodsPool::updatePool.push_back(UpdateWrapper(component));
}

template <class T>
typename std::enable_if<!HasUpdate<T>::value>::type
GameObject::onSceneUpdate(T * component) {
    addUpdate(component);
}

template <class T>
typename std::enable_if<HasFixedUpdate<T>::value>::type
GameObject::onSceneFixedUpdate(T * component) {
    component->fixedUpdatePosition = MethodsPool::fixedUpdatePool.size();    
    MethodsPool::fixedUpdatePool.push_back(FixedUpdateWrapper(component));
}

template <class T>
typename std::enable_if<!HasFixedUpdate<T>::value>::type
GameObject::onSceneFixedUpdate(T * component) {
    addFixedUpdate(component);
}

#endif //WG_GAMEOBJECT_H
