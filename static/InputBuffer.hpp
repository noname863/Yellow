#ifndef INPUTBUFFER_HPP
#define INPUTBUFFER_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include "../utility/ThreadSafeBitset.hpp"
#include <array>
#include "../linalg/Vector2.hpp"

class InputBuffer
{
    // sf::Keyboard::Return is last in int
    // enum, so it it + 1 is equal to numbers
    // of keys in enum
    static ThreadSafeBitset<sf::Keyboard::KeyCount> pressedKeys;
    static ThreadSafeBitset<sf::Keyboard::KeyCount> heldKeys;
    static ThreadSafeBitset<sf::Keyboard::KeyCount> releasedKeys;

    static ThreadSafeBitset<sf::Mouse::ButtonCount> pressedMouseButtons;
    static ThreadSafeBitset<sf::Mouse::ButtonCount> heldMouseButtons;
    static ThreadSafeBitset<sf::Mouse::ButtonCount> releasedMouseButtons;
    
    static std::array<std::pair<sf::Event::MouseButtonEvent, std::mutex>,
               sf::Mouse::ButtonCount> pressedMouseEvents;
    
    static std::array<std::pair<sf::Event::MouseButtonEvent, std::mutex>,
               sf::Mouse::ButtonCount> heldMouseEvents;
    
    static std::array<std::pair<sf::Event::MouseButtonEvent, std::mutex>,
               sf::Mouse::ButtonCount> releasedMouseEvents;
    
    static std::mutex wheelScrollMutex;
    static sf::Event::MouseWheelScrollEvent wheelScrollEvent;
    static bool wheelScrolled;

    static Vector2 mousePosition;
    static std::mutex mousePositionMutex;
    
public:

    static void addKeyPressed(sf::Event::KeyEvent keyEvent);
    static void addKeyReleased(sf::Event::KeyEvent keyEvent);

    static void addMouseButtonPressed(sf::Event::MouseButtonEvent buttonEvent);
    static void addMouseButtonReleased(sf::Event::MouseButtonEvent buttonEvent);

    static void addWheelScroll(sf::Event::MouseWheelScrollEvent mouseScrollEvent);
    
    static void setMousePosition(sf::Event::MouseMoveEvent mouseMoveEvent);
    
    static void reset();
    
    friend class Input;
};

#endif // INPUT_HPP
