//
// Created by adjsky on 1/1/21.
//

#include "AnimatedPlayer.h"

AnimatedPlayer::AnimatedPlayer(const std::unique_ptr<sf::Texture>& texture) :
    Player{ texture },
    period_{},
    currentAnimation_{ },
    textureRectangles_{ },
    animationCount_{ },
    timeElapsed_{ }
{
}

void AnimatedPlayer::setPeriod(float period) {
    period_ = period;
}

void AnimatedPlayer::setAnimation(AnimatedPlayer::Animations animation) {
    currentAnimation_ = animation;
}

void AnimatedPlayer::addAnimationFrame(AnimatedPlayer::Animations animation, sf::IntRect rect) {
    textureRectangles_[animation].push_back(rect);
}

void AnimatedPlayer::update(float delta) {
    if (currentAnimation_ != Animations::STAND) {
        timeElapsed_ += delta;
        if (timeElapsed_ > period_) {
            setTextureRect(textureRectangles_[currentAnimation_][animationCount_]);
            animationCount_ += 1;
            if (animationCount_ == textureRectangles_[currentAnimation_].size())
                animationCount_ = 0;
        }
    }
    else {
        if (horizontalSpeed > 0) {
            setTextureRect(textureRectangles_[currentAnimation_][0]);
        }
        if (horizontalSpeed < 0) {
            setTextureRect(textureRectangles_[currentAnimation_][1]);
        }
    }

}
