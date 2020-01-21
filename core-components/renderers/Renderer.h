//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_RENDERER_H
#define WG_RENDERER_H


#include <SFML/Graphics.hpp>
#include <vector>
#include "../../core/Component.h"

class Renderer : public Component {
private:
    static std::vector<Renderer*> renderers;
    size_t positionInRenderers;
    static bool needSort;
public:
    float priotity = 0;
    static void draw(sf::RenderTarget &target, sf::RenderStates states);

    void start();
    void onDestroy();
    virtual void onDraw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};


#endif //WG_RENDERER_H
