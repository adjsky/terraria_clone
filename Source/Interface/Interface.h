//
// Created by adjsky on 15.01.2021.
//

#ifndef TERRARIA_CLONE_INTERFACE_H
#define TERRARIA_CLONE_INTERFACE_H

#include <TGUI/TGUI.hpp>

#include "../Entities/Player/Player.h"
#include "../Util/entt.hpp"

class Interface {
public:
    explicit Interface(sf::RenderWindow& window);
    void handleEvent(sf::Event& e);
    void draw();

    bool gameInterfaceIsShown() const;
    bool inventoryIsOpen() const;
    bool consoleIsShown() const;
    bool hotBarIsShown() const;
    bool gameMenuIsOpen() const;

    void showGameInterface(bool condition);
    void showInventory(bool condition);
    void showHotBar(bool condition);
    void showConsole(bool condition);
    void showGameMenu(bool condition);
    void showMainMenu(bool condition);

    void updateHealth(const Player& player);
    void updateHotBar(const Player& player);
    void updateInventory(const Player& player);
    void updateAttachedItem(const Player& player, bool swapped = false);

    void highlightHotBarCell(const Player& player);

private:
    void constructMainMenu();
    void constructPlayMenu();
    void constructGameInterface();
    void constructHotBar();
    void constructHealthBar();
    void constructInventory();
    void constructConsole();
    void constructMenu();

    tgui::Gui gui_;
    sf::RenderWindow& window_;
};


#endif //TERRARIA_CLONE_INTERFACE_H