//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_GAME_H
#define TERRARIA_CLONE_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "../Util/constants.h"
#include "../Entities/Player/Player.h"

constexpr int GAME_SPEED = BLOCK_SIZE / 16;

class Game {
public:
    Game();
    void start();
    void update();
    void fixedUpdate();
    void handleEvents();
    void render();

private:
    void resizeWindow();

    float fixedDelta_;
    sf::RenderWindow window_;
    sf::View view_;
    Player player_;
    sf::Text fpsText_;
    sf::Text positionText_;
    bool noclip_;
    bool drawHitBoxes_;
};

#endif //TERRARIA_CLONE_GAME_H