//
// Created by Олег Бобров on 17.11.2019.
//

#include "Renderer.h"
#include <algorithm>

std::vector<Renderer*> Renderer::renderers;
bool Renderer::needSort = false;

void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) {
    if (needSort) {
        std::sort(Renderer::renderers.begin(), Renderer::renderers.end(), 
                  [](Renderer * first, Renderer * second) {
            return first->priotity > second->priotity;
        });
        for (size_t i = 0; i < Renderer::renderers.size(); ++i) {
            Renderer::renderers[i]->positionInRenderers = i;
        }
    }
    for (auto renderer : renderers) {
        renderer->onDraw(target, states);
    }
}

void Renderer::start() {
    positionInRenderers = Renderer::renderers.size();    
    Renderer::renderers.push_back(this);
    needSort = true;
}

void Renderer::onDestroy() {
    std::swap(Renderer::renderers[positionInRenderers], 
              Renderer::renderers.back());
    Renderer::renderers[positionInRenderers]->positionInRenderers = positionInRenderers;
    Renderer::renderers.pop_back();
    needSort = true;
}
