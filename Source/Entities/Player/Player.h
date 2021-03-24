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

class Player: public AnimatedSprite
{
public:
    enum AnimationTypes
    {
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
    Inventory& getHotBar();
    Inventory& getBackpack();
    const Inventory& getHotBar() const;
    const Inventory& getBackpack() const;
    int getHotBarIndex() const;
    void setHotBarIndex(int i);
    int getHealth() const;

    // serialization
    friend boost::serialization::access;
    template<class Archive>
    inline void save(Archive& ar, const unsigned int version) const
    {
        // can't get rid of a bug of saving and loading a local vector so i have to use floats instead
        float x{ getPosition().x };
        float y{ getPosition().y };
        ar & x;
        ar & y;
        ar & isOnGround;
        ar & hotBar_;
        ar & backpack_;
        ar & health_;
        ar & hotBarIndex_;
    }
    template<class Archive>
    inline void load(Archive& ar, const unsigned int version)
    {
        // can't get rid of a bug of saving and loading a local vector so i have to use floats instead
        float x;
        float y;
        ar & x;
        ar & y;
        move(x, y);
        ar & isOnGround;
        ar & hotBar_;
        ar & backpack_;
        ar & health_;
        ar & hotBarIndex_;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
public:
    bool isOnGround;
    float verticalSpeed;
    float horizontalSpeed;
    bool hasAttachedItem;
    Inventory::Cell attachedItem;

private:
    sf::RectangleShape hitBox_;
    std::array<Animation, ANIMATIONS_COUNT> animations_;
    Inventory hotBar_;
    Inventory backpack_;
    int hotBarIndex_;
    int health_;
};

#endif //TERRARIA_CLONE_PLAYER_H
