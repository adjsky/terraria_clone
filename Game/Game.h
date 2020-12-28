//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_GAME_H
#define TERRARIA_CLONE_GAME_H

#include <SFML/Graphics.hpp>

#include "../constants.h"

class Game {
private:
    sf::RenderWindow window;

    sf::View view;

    sf::Texture texture;
    sf::Sprite sprite;
public:
    explicit Game();
    void start();
    void update(sf::Time delta);
    void handleEvents();
    void render();
};


#endif //TERRARIA_CLONE_GAME_H
