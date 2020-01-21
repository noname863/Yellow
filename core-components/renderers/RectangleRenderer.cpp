#include "RectangleRenderer.hpp"

void RectangleRenderer::onDraw(sf::RenderTarget & target, sf::RenderStates states) const {
    states.transform *= transform->getLocalToWorldTransform();
    target.draw(rectangle, states);
}
