//
// Created by Олег Бобров on 19.01.2020.
//

#ifndef WG_LINEPHYSICS_HPP
#define WG_LINEPHYSICS_HPP


#include "../../core/GameObject.h"
#include "../Line.h"
#include "../Point.h"

class LinePhysics : public Component {
private:
    Line * line = nullptr;
public:
    void start();
    void fixedUpdate();
    void onDestroy();
    void onMapLayer10(const nlohmann::json & json);
};


#endif //WG_LINEPHYSICS_HPP
