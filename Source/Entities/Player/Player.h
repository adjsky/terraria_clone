//
// Created by kirill on 08.01.2021.
//

#ifndef TERRARIA_CLONE_PLAYER_H
#define TERRARIA_CLONE_PLAYER_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <array>

#include "../../Animations/AnimatedSprite.h"
#include "../Inventory/Inventory.h"
#include "../../World/World.h"

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

    void moveWithCollide(const World& world);
    float getDistanceToGround(const World& world) const;
    void constructHitBox();
    const sf::RectangleShape& getHitBox() const;
    void move(float x, float y);
    Animation& getAnimation(AnimationTypes type);
    Inventory& getInventory();
    const Inventory& getInventory() const;
    int getHeldItem() const;
    void setHeldItem(int i);

    int getHealth() const;

public:
    bool isOnGround;
    float verticalSpeed;
    float horizontalSpeed;

private:
    sf::RectangleShape hitBox_;
    std::array<Animation, ANIMATIONS_COUNT> animations_;
    Inventory inventory_;
    int heldItem_;
    int health_;
};

#endif //TERRARIA_CLONE_PLAYER_H
