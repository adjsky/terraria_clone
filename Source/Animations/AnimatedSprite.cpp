//
// Created by kirill on 08.01.2021.
//

#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(float timeStep) :
    vertices_{ },
    animation_{ nullptr },
    timeStep_{ timeStep },
    timePassed_{ }
{
}

void AnimatedSprite::setAnimation(Animation& animation)
{
    animation_ = &animation;
    updatePositions();
    updateTextureCoords();
}

void AnimatedSprite::setTimeStep(float timeStep)
{
    timeStep_ = timeStep;
}

void AnimatedSprite::update(float delta)
{
    if (animation_) {
        timePassed_ += delta;
        if (timePassed_ >= timeStep_) {
            animation_->next();
            updatePositions();
            updateTextureCoords();
            timePassed_ -= timeStep_;
        }
    }
}

void AnimatedSprite::changeDirection(AnimatedSprite::MoveDirections direction)
{
    float scaleX{ getScale().x };
    if (direction == MoveDirections::LEFT) {
        if (scaleX > 0) {
            scale(-1, 1);
        }
    }
    if (direction == MoveDirections::RIGHT) {
        if (scaleX < 0) {
            scale(-1, 1);
        }
    }
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
    auto width{ static_cast<float>(std::abs(animation_->getFrame().width)) };
    auto height{ static_cast<float>(std::abs(animation_->getFrame().height)) };

    return sf::FloatRect{ 0.0f, 0.0f, width, height };
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

void AnimatedSprite::updatePositions()
{
    sf::FloatRect bounds{ getLocalBounds() };

    vertices_[0].position = sf::Vector2f{ 0.0f, 0.0f };
    vertices_[1].position = sf::Vector2f{ 0.0f, bounds.height };
    vertices_[2].position = sf::Vector2f{ bounds.width, 0 };
    vertices_[3].position = sf::Vector2f{ bounds.width, bounds.height };
}

void AnimatedSprite::updateTextureCoords()
{
    auto left{ static_cast<float>(animation_->getFrame().left) };
    float right{ left + animation_->getFrame().width };
    auto top{ static_cast<float>(animation_->getFrame().top) };
    float bottom{ top + animation_->getFrame().height };

    vertices_[0].texCoords = sf::Vector2f{ left, top };
    vertices_[1].texCoords = sf::Vector2f{ left, bottom };
    vertices_[2].texCoords = sf::Vector2f{ right, top };
    vertices_[3].texCoords = sf::Vector2f{ right, bottom };
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (animation_) {
        states.transform *= getTransform();
        states.texture = animation_->getSpriteSheet();
        target.draw(vertices_, 4, sf::TriangleStrip, states);
    }
}