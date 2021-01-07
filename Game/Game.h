//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_GAME_H
#define TERRARIA_CLONE_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "../Util/constants.h"
#include "../Entities/Player/AnimatedPlayer/AnimatedPlayer.h"

constexpr int GAME_SPEED = BLOCK_SIZE / 16;

class Game {
private:
    float fixedDelta;
    sf::RenderWindow window_;
    sf::View view_;
    AnimatedPlayer player_;
    sf::Text fpsText_;
    sf::Text positionText_;
    bool noclip_;
public:
    Game();
    void start();
    void update();
    void fixedUpdate();
    void handleEvents();
    void render();
};


#endif //TERRARIA_CLONE_GAME_H
