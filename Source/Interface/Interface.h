//
// Created by adjsky on 15.01.2021.
//

#ifndef TERRARIA_CLONE_INTERFACE_H
#define TERRARIA_CLONE_INTERFACE_H

#include <TGUI/TGUI.hpp>

#include "../Entities/Player/Player.h"
#include "../Signals/Signal.h"

class Interface {
public:
    explicit Interface(sf::RenderWindow& window);
    void handleEvent(sf::Event& e);
    void draw();

    void showInventory();
    void showHotBar();
    void showConsole();
    void updateHealth(const Player& player);
    void updateHotBar(const Player& player);
    void updateInventory(const Player& player);
    void highlightHotBarCell(const Player& player);

public:
    Signal<std::string> consoleEnterSignal;
    Signal<int, int> backpackCellPressed;
    Signal<int> hotBarCellPressed;

private:
    void constructHotBar();
    void constructHealthBar();
    void constructInventory();
    void constructConsole();

    tgui::Gui gui_;
    sf::RenderWindow& window_;
};


#endif //TERRARIA_CLONE_INTERFACE_H