//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_LINE_H
#define WG_LINE_H


#include "Behaviour.h"
#include "../core-components/renderers/LineRenderer.h"
#include "../utility/Config.hpp"

class Line : public Behaviour {
private:
    LineRenderer * lineRenderer = nullptr;
public:
    int length = -1;
    Point * points[2] = {nullptr, nullptr};

    explicit Line(int idx);

    void start();
    void update();
    void applyLayer0(const nlohmann::json &json) override;
};

#endif //WG_LINE_H
