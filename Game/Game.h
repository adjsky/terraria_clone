//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_GAME_H
#define TERRARIA_CLONE_GAME_H

#include <SFML/Graphics.hpp>

#include "../Util/constants.h"
#include "../World/World.h"

class Game {
private:
    sf::RenderWindow window;
    sf::View view;
    sf::Vector2f player;
    World world;
    sf::Text fpsText;
public:
    explicit Game();
    void start();
    void update(float delta);
    void handleEvents();
    void render();

private:
    sf::Vector2i mapGlobalCoordsToGame(sf::Vector2f& globalCoords);
};


#endif //TERRARIA_CLONE_GAME_H
