//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_GAME_H
#define TERRARIA_CLONE_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "../Util/constants.h"
#include "../Entities/Player/Player.h"
#include "../Interface/Interface.h"

constexpr int GAME_SPEED = BLOCK_SIZE / 16;

class Game {
public:
    Game(const sf::ContextSettings& context = sf::ContextSettings{});
    void start();
    void update();
    void fixedUpdate();
    void handleEvents();
    void render();

private:
    void resizeWindow();
    void handleMouseClicks();

    float fixedDelta_;
    sf::RenderWindow window_;
    sf::View view_;
    Player player_;
    bool noclip_;
    bool drawHitBoxes_;
    bool paused_;
    Interface gui_;
};

#endif //TERRARIA_CLONE_GAME_H