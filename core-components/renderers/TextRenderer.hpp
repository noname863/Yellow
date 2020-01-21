#ifndef TEXTRENDERER_HPP
#define TEXTRENDERER_HPP

#include "Renderer.h"

class TextRenderer : public Renderer {
public:
    sf::Text text;
    void onDraw(sf::RenderTarget &target, sf::RenderStates states) const override; 
};

#endif // TEXTRENDERER_HPP
