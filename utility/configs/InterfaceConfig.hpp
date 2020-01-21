#ifndef INTERFACECONFIG_HPP
#define INTERFACECONFIG_HPP
#include <SFML/Graphics.hpp>

class InterfaceConfig
{
public:
    static const float largeR;
    static const float smallR;
    static const sf::Color graphColor;
    static const sf::Color playerTownColor;
    static const sf::Color playerTrainColor;
    static const sf::Color townColor;
    static const sf::Color storageColor;
    static const sf::Color marketColor;
    static const sf::Color trainColor;
    
    static const sf::Color buttonColor;
    static const float buttonDarkFloat;
    static const std::string fontFileName;
    static sf::Font * textFont;
    
};

#endif // INTERFACECONFIG_HPP
