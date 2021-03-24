//
// Created by kirill on 08.01.2021.
//

#include "Animation.h"

Animation::Animation() :
    spriteSheet_{ nullptr },
    textureRectangles_{ },
    currentFrame_{ }
{

}

Animation::Animation(const sf::Texture& texture) :
    spriteSheet_{ &texture },
    textureRectangles_{ },
    currentFrame_{ }
{

}

void Animation::setSpriteSheet(const sf::Texture& texture)
{
    spriteSheet_ = &texture;
}

const sf::Texture* Animation::getSpriteSheet() const
{
    return spriteSheet_;
}

const sf::IntRect& Animation::getFrame() const
{
    return textureRectangles_[currentFrame_];
}

void Animation::next()
{
    currentFrame_++;
    if (currentFrame_ == textureRectangles_.size()) {
        currentFrame_ = 0;
    }
}

void Animation::addFrame(sf::IntRect rect)
{
    textureRectangles_.push_back(rect);
}
