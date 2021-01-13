//
// Created by kirill on 08.01.2021.
//

#ifndef TERRARIA_CLONE_PLAYER_H
#define TERRARIA_CLONE_PLAYER_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <array>

#include "../../Animations/AnimatedSprite.h"
#include "../Inventory/Inventory.h"

constexpr int PLAYER_WIDTH = 45;
constexpr int PLAYER_HEIGHT = 90;

class Player : public AnimatedSprite {
public:
    enum AnimationTypes {
        MOVE,
        STAND,
        JUMP,
        ANIMATIONS_COUNT
    };
public:
    Player();

    void moveWithCollide();
    float getDistanceToGround() const;
    void constructHitBox();
    const sf::RectangleShape& getHitBox() const;
    void move(float x, float y);
    Animation& getAnimation(AnimationTypes type);
    void updateStates();
    Inventory& getHotBar();
    int getHeldItem();

public:
    bool isOnGround;
    float verticalSpeed;
    float horizontalSpeed;

private:
    sf::RectangleShape hitBox_;
    std::array<Animation, ANIMATIONS_COUNT> animations_;
    Inventory hotBar_;
    int heldItem_;
};

#endif //TERRARIA_CLONE_PLAYER_H
