//
// Created by adjsky on 1/1/21.
//

#include "AnimatedPlayer.h"

#include <iostream>

AnimatedPlayer::AnimatedPlayer(const std::unique_ptr<sf::Texture>& texture) :
    Player{ texture },
    period_{},
    currentAnimation_{ },
    moveDirection{ MoveDirection::RIGHT },
    textureRectangles_{ },
    animationCount_{ },
    timeElapsed_{ }
{
}

void AnimatedPlayer::setPeriod(float seconds) {
    period_ = seconds;
}

void AnimatedPlayer::setAnimation(AnimatedPlayer::Animations type) {
    currentAnimation_ = type;
}

void AnimatedPlayer::addAnimationFrame(AnimatedPlayer::Animations type, sf::IntRect rect) {
    textureRectangles_[type].push_back(rect);
}

void AnimatedPlayer::update(float delta) {
    if (currentAnimation_ == Animations::STAND) {
        if (moveDirection == MoveDirection::RIGHT) {
            setTextureRect(textureRectangles_[currentAnimation_][0]);
        }
        if (moveDirection == MoveDirection::LEFT) {
            setTextureRect(textureRectangles_[currentAnimation_][1]);
        }
    }
    else {
        timeElapsed_ += delta;
        if (timeElapsed_ > period_) {
            setTextureRect(textureRectangles_[currentAnimation_][animationCount_]);
            animationCount_ += 1;
            if (animationCount_ == textureRectangles_[currentAnimation_].size())
                animationCount_ = 0;
            timeElapsed_ = 0;
        }
    }
}

void AnimatedPlayer::setDirection(Player::MoveDirection direction) {
    moveDirection = direction;
}
