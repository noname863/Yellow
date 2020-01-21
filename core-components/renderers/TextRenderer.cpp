#include "TextRenderer.hpp"
#include "../../static/Time.h"

void TextRenderer::onDraw(sf::RenderTarget & target, sf::RenderStates states) const {
    states.transform *= transform->getLocalToWorldTransform();
    target.draw(text, states);
}
