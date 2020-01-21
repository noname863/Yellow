#ifndef METHODSPOOL_HPP
#define METHODSPOOL_HPP

#include <list>
#include <vector>
#include "../utility/methodWrapper.hpp"

class GameObject;
class Component;

class MethodsPool
{
    friend class GameObject;
    friend class Component;
    static std::list<StartWrapper> startPool;
    static std::vector<UpdateWrapper> updatePool;
    static std::vector<FixedUpdateWrapper> fixedUpdatePool;
    static std::list<GameObject *> destroyObjectPool;
    static std::list<Component *> destroyComponentPool;
    static void removeFromUpdate(size_t index);
    static void removeFromFixedUpdate(size_t index);
public:
    static void start();
    static void update();
    static void fixedUpdate();
    static void onDestroy();
};

#endif // METHODSPOOL_HPP
