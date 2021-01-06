//
// Created by adjsky on 1/1/21.
//

#include "AnimatedPlayer.h"
#include <iostream>

AnimatedPlayer::AnimatedPlayer(const std::unique_ptr<sf::Texture>& texture) :
    Player{ texture },
    period_{},
    currentAnimation_{ STAND },
    textureRectangles_{ },
    animationCount_{ },
    timeElapsed_{ }
{
}

void AnimatedPlayer::setPeriod(float seconds) {
    period_ = seconds;
}

void AnimatedPlayer::setAnimation(AnimatedPlayer::Animations type) {
    if (currentAnimation_ != type) {
        animationCount_ = 0;
        currentAnimation_ = type;
    }
}

void AnimatedPlayer::addAnimationFrame(AnimatedPlayer::Animations type, sf::IntRect rect) {
    textureRectangles_[type].push_back(rect);
}

void AnimatedPlayer::update(float delta) {
    timeElapsed_ += delta;
    if (timeElapsed_ > period_) {
        setTextureRect(textureRectangles_[currentAnimation_][animationCount_]);
        animationCount_ += 1;
        if (animationCount_ >= textureRectangles_[currentAnimation_].size())
            animationCount_ = 0;
        timeElapsed_ = 0;
    }
}

void AnimatedPlayer::setAnimationDirection(Player::MoveDirection direction) {
    sf::Vector2f scale = player_.getScale();
    if (direction == LEFT) {
        if (scale.x > 0) {
            player_.setScale(-scale.x, scale.y);
        }
    }
    if (direction == RIGHT){
        if (scale.x < 0) {
            player_.setScale(-scale.x, scale.y);
        }
    }
}
