#include "InputBuffer.hpp"
#include <iostream>
//for keys
ThreadSafeBitset<sf::Keyboard::KeyCount> InputBuffer::pressedKeys;
ThreadSafeBitset<sf::Keyboard::KeyCount> InputBuffer::heldKeys;
ThreadSafeBitset<sf::Keyboard::KeyCount> InputBuffer::releasedKeys;

//for mouse buttons
ThreadSafeBitset<sf::Mouse::ButtonCount> InputBuffer::pressedMouseButtons;
ThreadSafeBitset<sf::Mouse::ButtonCount> InputBuffer::heldMouseButtons;
ThreadSafeBitset<sf::Mouse::ButtonCount> InputBuffer::releasedMouseButtons;
std::array<std::pair<sf::Event::MouseButtonEvent, std::mutex>,
        sf::Mouse::ButtonCount> InputBuffer::pressedMouseEvents;
std::array<std::pair<sf::Event::MouseButtonEvent, std::mutex>,
        sf::Mouse::ButtonCount> InputBuffer::heldMouseEvents;
std::array<std::pair<sf::Event::MouseButtonEvent, std::mutex>,
        sf::Mouse::ButtonCount> InputBuffer::releasedMouseEvents;

std::mutex InputBuffer::wheelScrollMutex;
sf::Event::MouseWheelScrollEvent InputBuffer::wheelScrollEvent;
bool InputBuffer::wheelScrolled = false;

Vector2 InputBuffer::mousePosition;
std::mutex InputBuffer::mousePositionMutex;

void InputBuffer::addKeyPressed(sf::Event::KeyEvent keyEvent) {
    if (keyEvent.code >= 0) {
        pressedKeys.set(static_cast<size_t>(keyEvent.code));
        heldKeys.set(static_cast<size_t>(keyEvent.code));
    }
    // another case will trigger if 
    // sfml don't know what is this button
}

void InputBuffer::addKeyReleased(sf::Event::KeyEvent keyEvent) {
    if (keyEvent.code >= 0) {
        releasedKeys.set(static_cast<size_t>(keyEvent.code));
        heldKeys.reset(static_cast<size_t>(keyEvent.code));
    }
}

void InputBuffer::addMouseButtonPressed(sf::Event::MouseButtonEvent buttonEvent) {
    pressedMouseEvents[buttonEvent.button].second.lock();
    heldMouseEvents[buttonEvent.button].second.lock();
    pressedMouseButtons.set(buttonEvent.button);
    heldMouseButtons.set(buttonEvent.button);
    pressedMouseEvents[buttonEvent.button].first = buttonEvent;
    heldMouseEvents[buttonEvent.button].first = buttonEvent;
    heldMouseEvents[buttonEvent.button].second.unlock();    
    pressedMouseEvents[buttonEvent.button].second.unlock();
}

void InputBuffer::addMouseButtonReleased(sf::Event::MouseButtonEvent buttonEvent) {
    releasedMouseButtons.set(buttonEvent.button);
    heldMouseButtons.reset(buttonEvent.button);
    releasedMouseEvents[buttonEvent.button].second.lock();
    releasedMouseEvents[buttonEvent.button].first = buttonEvent;
    releasedMouseEvents[buttonEvent.button].second.unlock();
}

void InputBuffer::addWheelScroll(sf::Event::MouseWheelScrollEvent mouseScrollEvent) {
    std::lock_guard<std::mutex> lock(wheelScrollMutex);
    wheelScrolled = true;
    InputBuffer::wheelScrollEvent = mouseScrollEvent;
}

void InputBuffer::setMousePosition(sf::Event::MouseMoveEvent mouseMoveEvent) {
    std::lock_guard<std::mutex> lock(mousePositionMutex);    
    mousePosition = {mouseMoveEvent.x, mouseMoveEvent.y};
}

void InputBuffer::reset() {
    pressedKeys.reset();
    releasedKeys.reset();
    pressedMouseButtons.reset();
    releasedMouseButtons.reset();
    std::lock_guard<std::mutex> lock(wheelScrollMutex);    
    wheelScrolled = false;
}
