#include <algorithm>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>
#include "static/Database.h"
#include "core/GameObject.h"
#include "core-components/Camera.h"
#include "core-components/renderers/Renderer.h"
#include "core-components/renderers/CircleRenderer.h"
#include "static/Prefabs.h"
#include "static/Time.h"
#include "network/PacketQueue.hpp"
#include "static/Input.hpp"
#include "static/InputBuffer.hpp"
#include "network/Network.hpp"
//TODO MAIN, may be hashmap in methodsPool, mouse scroll.

struct MainLoopIteration {
    sf::RenderWindow & window; 
    Camera * mainCamera;
    sf::Clock & clock;
    float & time;
    
    void operator()() {
        // here is main loop iteration
        Time::deltaTime = clock.restart().asSeconds();
        Input::setFromInputBuffer();
        MethodsPool::start();
        MethodsPool::update();
        time += Time::deltaTime;
        while (time >= Time::fixedDeltaTime) {
            MethodsPool::fixedUpdate();
            time -= Time::fixedDeltaTime;
        }
        Network::update();
        MethodsPool::onDestroy();
        window.clear();
        Renderer::draw(window, mainCamera->getRenderState());
        window.display();
    }
};


void mainLoop(MainLoopIteration & mainLoopIteration, 
              std::atomic<bool> & closeCalled) {

    mainLoopIteration.window.setActive(true);

    mainLoopIteration.clock.restart(); 
    mainLoopIteration.time = 0.f;
    
    while (!closeCalled.load(std::memory_order_acquire)) {
        mainLoopIteration();
    }

}

int main() {
    InterfaceConfig::textFont = new sf::Font;
    InterfaceConfig::textFont->loadFromFile(InterfaceConfig::fontFileName);
    
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Graph");
    window.setActive(true);
    
    GameObject * root = Prefabs::root()->instantiate();
    Camera * mainCamera = Prefabs::camera(&window);
    mainCamera->gameObject->instantiate();

    sf::Clock clock; // starts the clock
    float time = 0.f;
    MainLoopIteration mainLoopIteration{window, mainCamera, clock, time};
    
#ifdef _WIN32
    std::atomic<bool> closeCalled;
    closeCalled.store(false, std::memory_order_release);
    window.setActive(false);
    std::thread mainLoopThread(mainLoop, std::ref(mainLoopIteration), std::ref(closeCalled));
#endif
    
    sf::Event event{};
    
    while (window.isOpen()) {

        if (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
#ifdef _WIN32
                closeCalled.store(true, std::memory_order_release);
                mainLoopThread.join();
#endif
                root->destroyImmediate();
                mainCamera->gameObject->destroyImmediate();
                delete InterfaceConfig::textFont;
                window.close();
                return 0;
            case sf::Event::Resized:
                mainCamera->onWindowResized();
                break;
            case sf::Event::KeyPressed:
                InputBuffer::addKeyPressed(event.key);
                break;
            case sf::Event::KeyReleased:
                InputBuffer::addKeyReleased(event.key);
                break;
            case sf::Event::MouseButtonPressed:
                InputBuffer::addMouseButtonPressed(event.mouseButton);
                break;
            case sf::Event::MouseButtonReleased:
                InputBuffer::addMouseButtonReleased(event.mouseButton);
                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    InputBuffer::addWheelScroll(event.mouseWheelScroll);
                    // std::cout << event.mouseWheelScroll.delta << std::endl;
                }
                break;
            case sf::Event::MouseMoved:
                InputBuffer::setMousePosition(event.mouseMove);
            default:
                break;
            }

        }
#ifndef _WIN32
        mainLoopIteration();
#endif
    }

    return 0;
}
