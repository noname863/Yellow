//
// Created by Олег Бобров on 20.01.2020.
//

#ifndef WG_MENUCONTROLLER_HPP
#define WG_MENUCONTROLLER_HPP


#include "../../core/GameObject.h"
#include "../Game.hpp"
#include "../../utility/json.hpp"
#include "../../core-components/ui/Button.hpp"
#include <iostream>

class MenuController : public Component {
private:
    float timeSinceRefresh = 0.f;
    nlohmann::json games;
    Button * hostButton = nullptr;
    std::vector<Button *> buttons;
    void filterGames();
    void initButtons();
    void initHostButton();
public:
    float refreshTime = 1.f;
    void start();
    void update();
    void onDestroy();

    void onGameSelected(int idx);
    void onGames(const nlohmann::json & json);
};


#endif //WG_MENUCONTROLLER_HPP
