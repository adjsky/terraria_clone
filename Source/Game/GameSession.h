//
// Created by adjsky on 18.01.2021.
//

#ifndef TERRARIA_CLONE_GAMESESSION_H
#define TERRARIA_CLONE_GAMESESSION_H

#include "../Interface/Interface.h"
#include "../World/World.h"

class GameSession {
public:
    GameSession(sf::RenderWindow& window, Interface& gui);

    void update(float delta);
    void fixedUpdate(float fixedDelta);
    void render();

private:
    sf::RenderWindow& window_;
    Interface& gui_;
    Player player_;
    sf::View view_;
    World world_;
    bool noclip_;
    bool drawHitBoxes_;
    bool paused_;
};


#endif //TERRARIA_CLONE_GAMESESSION_H