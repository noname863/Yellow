#include "Input.hpp"
#include <iostream>
#include "InputBuffer.hpp"

//for keys
std::bitset<sf::Keyboard::KeyCount> Input::pressedKeys;
std::bitset<sf::Keyboard::KeyCount> Input::heldKeys;
std::bitset<sf::Keyboard::KeyCount> Input::releasedKeys;

//for mouse buttons
std::bitset<sf::Mouse::ButtonCount> Input::pressedMouseButtons;
std::bitset<sf::Mouse::ButtonCount> Input::heldMouseButtons;
std::bitset<sf::Mouse::ButtonCount> Input::releasedMouseButtons;

std::array<sf::Event::MouseButtonEvent, 
        sf::Mouse::ButtonCount> Input::pressedMouseEvents;

std::array<sf::Event::MouseButtonEvent, 
        sf::Mouse::ButtonCount> Input::heldMouseEvents;

std::array<sf::Event::MouseButtonEvent, 
        sf::Mouse::ButtonCount> Input::releasedMouseEvents;

sf::Event::MouseWheelScrollEvent Input::wheelScrollEvent;
bool Input::wheelScrolled = false;

Vector2 Input::mousePosition;

bool Input::isKeyDown(sf::Keyboard::Key key) {
    // assume you will not ask sf::Keybord::Unknown
    return pressedKeys[static_cast<size_t>(key)];
}

bool Input::isKeyUp(sf::Keyboard::Key key) {
    return releasedKeys[static_cast<size_t>(key)];
}

bool Input::isKey(sf::Keyboard::Key key) {
    return heldKeys[static_cast<size_t>(key)];
}

bool Input::getMouseButtonPressed(sf::Mouse::Button button) {
    return pressedMouseButtons[button];
}

bool Input::getMouseButtonHeld(sf::Mouse::Button button) {
    return heldMouseButtons[button];
}

bool Input::getMouseButtonReleased(sf::Mouse::Button button) {
    return releasedMouseButtons[button];
}

sf::Event::MouseButtonEvent Input::getMBPressedEvent(sf::Mouse::Button button) {
    return pressedMouseEvents[button];
}

sf::Event::MouseButtonEvent Input::getMBHeldEvent(sf::Mouse::Button button) {
    return heldMouseEvents[button];
}

sf::Event::MouseButtonEvent Input::getMBReleasedEvent(sf::Mouse::Button button) {
    return releasedMouseEvents[button];
}

void Input::setFromInputBuffer() {
    
    size_t i = 0;
    size_t inBitsetIndex = 0;
    for (; i < sf::Keyboard::KeyCount; ++inBitsetIndex) {
        
        size_t end = i + 32;
        
        InputBuffer::pressedKeys.getMutex(inBitsetIndex).lock();
        InputBuffer::heldKeys.getMutex(inBitsetIndex).lock();
        InputBuffer::releasedKeys.getMutex(inBitsetIndex).lock();    
        
        for (; (i < sf::Keyboard::KeyCount) && (i < end); ++i) {
            Input::pressedKeys.set(i, InputBuffer::pressedKeys.unsafeGet(i));
            Input::heldKeys.set(i, InputBuffer::heldKeys.unsafeGet(i));
            Input::releasedKeys.set(i, InputBuffer::releasedKeys.unsafeGet(i));
        }
        
        InputBuffer::pressedKeys.unsafeSetNumber(inBitsetIndex, 0);
        InputBuffer::releasedKeys.unsafeSetNumber(inBitsetIndex, 0);
        
        InputBuffer::pressedKeys.getMutex(inBitsetIndex).unlock();
        InputBuffer::heldKeys.getMutex(inBitsetIndex).unlock();
        InputBuffer::releasedKeys.getMutex(inBitsetIndex).unlock();   
    }
    i = 0;
    inBitsetIndex = 0;
    
    for (; i < sf::Mouse::ButtonCount; ++inBitsetIndex) {
        
        size_t end = i + 32;
        
        InputBuffer::pressedMouseButtons.getMutex(inBitsetIndex).lock();
        InputBuffer::heldMouseButtons.getMutex(inBitsetIndex).lock();
        InputBuffer::releasedMouseButtons.getMutex(inBitsetIndex).lock();
        
        for (; (i < sf::Mouse::ButtonCount) && (i < end); ++i) {
            
            Input::pressedMouseButtons.set(i, InputBuffer::pressedMouseButtons.unsafeGet(i));
            Input::releasedMouseButtons.set(i, InputBuffer::releasedMouseButtons.unsafeGet(i));
            Input::heldMouseButtons.set(i, InputBuffer::heldMouseButtons.unsafeGet(i));
            
            
            InputBuffer::pressedMouseEvents[i].second.lock();
            InputBuffer::heldMouseEvents[i].second.lock();
            InputBuffer::releasedMouseEvents[i].second.lock();
            
            Input::pressedMouseEvents[i] = InputBuffer::pressedMouseEvents[i].first;
            Input::releasedMouseEvents[i] = InputBuffer::releasedMouseEvents[i].first;            
            Input::heldMouseEvents[i] = InputBuffer::heldMouseEvents[i].first;            
            
            InputBuffer::pressedMouseEvents[i].second.unlock();
            InputBuffer::heldMouseEvents[i].second.unlock();
            InputBuffer::releasedMouseEvents[i].second.unlock();

        }
        
        InputBuffer::pressedMouseButtons.unsafeSetNumber(inBitsetIndex, 0);
        InputBuffer::releasedMouseButtons.unsafeSetNumber(inBitsetIndex, 0);
        
        InputBuffer::pressedMouseButtons.getMutex(inBitsetIndex).unlock();
        InputBuffer::heldMouseButtons.getMutex(inBitsetIndex).unlock();        
        InputBuffer::releasedMouseButtons.getMutex(inBitsetIndex).unlock();
        
    }
    InputBuffer::wheelScrollMutex.lock();
    
    Input::wheelScrollEvent = InputBuffer::wheelScrollEvent;
    Input::wheelScrolled = InputBuffer::wheelScrolled;
    InputBuffer::wheelScrolled = false;
    
    InputBuffer::wheelScrollMutex.unlock();
    
    InputBuffer::mousePositionMutex.lock();
    Input::mousePosition = InputBuffer::mousePosition;
    InputBuffer::mousePositionMutex.unlock();
    
}
