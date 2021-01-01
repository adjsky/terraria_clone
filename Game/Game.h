//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_GAME_H
#define TERRARIA_CLONE_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "../Util/constants.h"
#include "../Entities/Player/AnimatedPlayer/AnimatedPlayer.h"

class Game {
private:
    sf::RenderWindow window_;
    sf::View view_;
    AnimatedPlayer player_;
    sf::Text fpsText_;
public:
    explicit Game();
    void start();
    void update(float delta);
    void updatePlayer(float delta);
    void handleClick();
    void updateCamera();
    void handleEvents();
    void render();
};


#endif //TERRARIA_CLONE_GAME_H
