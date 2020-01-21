#include "InterfaceConfig.hpp"

const float InterfaceConfig::largeR = 1.0f;
const float InterfaceConfig::smallR = 0.8f * largeR;
const sf::Color InterfaceConfig::graphColor = {190, 190, 190};
const sf::Color InterfaceConfig::playerTownColor = {190, 190, 0};
const sf::Color InterfaceConfig::playerTrainColor = {250, 250, 0};
const sf::Color InterfaceConfig::townColor = {0, 190, 0};
const sf::Color InterfaceConfig::storageColor = {0, 0, 190};
const sf::Color InterfaceConfig::marketColor = {190, 0, 0};
const sf::Color InterfaceConfig::trainColor = {0, 250, 0};

const sf::Color InterfaceConfig::buttonColor = {255, 255, 255};
const float InterfaceConfig::buttonDarkFloat = 0.75f;
const std::string InterfaceConfig::fontFileName = "Menlo-Regular.ttf";
sf::Font * InterfaceConfig::textFont;
