//
// Created by Олег Бобров on 08.12.2019.
//

#ifndef WG_EVENT_HPP
#define WG_EVENT_HPP

#include <list>
#include <algorithm>


template <class... Args>
class Event {
private:
    std::list <std::pair<void *, void (*)(void *, Args...)>> listeners;

    template<class TClass, void (TClass::*Method)(Args...)>
    static void getMethod(void * objPtr, Args... args);
public:
    template<class TClass, void (TClass::*Method)(Args...)>
    void addListener(TClass *objPtr);
    template<class TClass, void (TClass::*Method)(Args...)>
    void removeListener(TClass *objPtr);
    void invoke(Args... arg);
};

template<class... Args>
template<class TClass, void (TClass::*Method)(Args...)>
void Event<Args...>::getMethod(void * objPtr, Args... args) {
    (static_cast<TClass*>(objPtr)->*Method)(std::forward<Args>(args)...);
}

template<class... Args>
template<class TClass, void (TClass::*Method)(Args...)>
void Event<Args...>::addListener(TClass *objPtr) {
    listeners.emplace_back(objPtr, &Event<Args...>::getMethod<TClass, Method>);
}

template<class... Args>
template<class TClass, void (TClass::*Method)(Args...)>
void Event<Args...>::removeListener(TClass *objPtr) {
    listeners.erase(find_if(listeners.begin(), listeners.end(),
                                 [&objPtr](std::pair<void *, void (*)(void *, Args...)> &pair) {
                                     return pair.first == objPtr &&
                                            pair.second == &Event<Args...>::getMethod<TClass, Method>;
                                 }));
}

template<class... Args>
void Event<Args...>::invoke(Args... args) {
    typename std::list <std::pair<void *, void (*)(void *, Args...)>>::iterator it, next;
    for (it = listeners.begin(); it != listeners.end(); it = next) {
        next = std::next(it);
        it->second(it->first, std::forward<Args>(args)...);
    }
}


#endif //WG_EVENT_HPP
