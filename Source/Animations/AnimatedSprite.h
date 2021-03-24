//
// Created by kirill on 08.01.2021.
//

#ifndef TERRARIA_CLONE_ANIMATEDSPRITE_H
#define TERRARIA_CLONE_ANIMATEDSPRITE_H

#include <SFML/Graphics.hpp>

#include "Animation.h"

class AnimatedSprite: public sf::Drawable, public sf::Transformable
{
public:
    explicit AnimatedSprite(float timeStep = 0);

    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

public:
    enum class MoveDirections
    {
        RIGHT,
        LEFT
    };

    void setAnimation(Animation& animation);
    void setTimeStep(float timeStep);
    void update(float delta);
    void changeDirection(MoveDirections direction);

private:
    void updatePositions();
    void updateTextureCoords();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // private members
    sf::Vertex vertices_[4];
    Animation* animation_;
    float timeStep_;
    float timePassed_;
};

#endif //TERRARIA_CLONE_ANIMATEDSPRITE_H
