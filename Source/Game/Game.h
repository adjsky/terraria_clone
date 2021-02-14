//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_GAME_H
#define TERRARIA_CLONE_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "GameSession.h"
#include "../Entities/Player/Player.h"
#include "../Interface/Interface.h"
#include "../GameLogic/GameLogic.h"

class Game {
public:
    Game();
    void start();
    void close();

    GameSession* getGameSession();
    GameSession* createGameSession();
    void deleteGameSession();

    sf::View& getCamera();

    void drawHitBoxes(bool condition);
    void pause(bool condition);
    bool isPaused() const;

private:
    void handleEvents();
    void render();

    float fixedDelta_;
    std::unique_ptr<GameSession> currentGameSession_;
    GameLogic gameLogic_;
    sf::View camera_;
    bool drawHitBoxes_;
    bool paused_;
};

#endif //TERRARIA_CLONE_GAME_H