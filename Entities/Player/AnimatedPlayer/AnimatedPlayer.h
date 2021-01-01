//
// Created by adjsky on 1/1/21.
//

#ifndef TERRARIA_CLONE_ANIMATEDPLAYER_H
#define TERRARIA_CLONE_ANIMATEDPLAYER_H

#include <array>
#include <vector>

#include "../Player.h"

class AnimatedPlayer : public Player {
public:
    enum Animations {
        LEFT = 0,
        RIGHT,
        JUMP,
        STAND,
        ANIMATIONS_COUNT
    };

public:
    explicit AnimatedPlayer(const std::unique_ptr<sf::Texture>& texture);
    void setPeriod(float period);
    void setAnimation(Animations animation);
    void addAnimationFrame(Animations animation, sf::IntRect rect);
    void update(float delta);

private:
    float timeElapsed_;
    float period_;
    int animationCount_;
    Animations currentAnimation_;

    std::array<std::vector<sf::IntRect>, ANIMATIONS_COUNT> textureRectangles_;
};


#endif //TERRARIA_CLONE_ANIMATEDPLAYER_H
