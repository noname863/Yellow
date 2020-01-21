//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_PREFABS_H
#define WG_PREFABS_H

#include "../core/GameObject.h"
#include "../custom-components/Point.h"
#include "../custom-components/Post.h"
#include "../custom-components/Train.h"
#include "../custom-components/Line.h"
#include "../core-components/Camera.h"
#include "../utility/Config.hpp"
#include "../core-components/ui/Button.hpp"

class Prefabs {
public:
    static Camera *camera(sf::RenderTarget *renderTarget);
    static Point *point(int idx);
    static Line *line(int idx);
    static Train *train(int idx);
    static Town *town(int idx);
    static Market *market(int idx);
    static Storage *storage(int idx);
    static Button * button(float width, float height, std::string text);
    static GameObject * root();
};


#endif //WG_PREFABS_H
