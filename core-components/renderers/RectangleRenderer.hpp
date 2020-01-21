#ifndef RECTANGLERENDERER_HPP
#define RECTANGLERENDERER_HPP

#include "Renderer.h"

class RectangleRenderer : public Renderer {
public:
    sf::RectangleShape rectangle;

    void onDraw(sf::RenderTarget &target, sf::RenderStates states) const override;     
};

#endif // RECTANGLERENDERER_HPP
