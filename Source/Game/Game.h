//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_GAME_H
#define TERRARIA_CLONE_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "GameSession.h"
#include "../Util/constants.h"
#include "../Entities/Player/Player.h"
#include "../Interface/Interface.h"

class Game {
public:
    Game(const sf::ContextSettings& context = sf::ContextSettings{});
    void start();
    void handleEvents();
    void render();

private:
    void resizeWindow();

    float fixedDelta_;
    sf::RenderWindow window_;
    Interface gui_;
    std::unique_ptr<GameSession> currentGameSession;
};

#endif //TERRARIA_CLONE_GAME_H