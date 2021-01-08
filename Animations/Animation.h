//
// Created by kirill on 08.01.2021.
//

#ifndef TERRARIA_CLONE_ANIMATION_H
#define TERRARIA_CLONE_ANIMATION_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

class Animation {
public:
    Animation();
    explicit Animation(const sf::Texture& texture);

    void setSpriteSheet(const sf::Texture& texture);
    const sf::Texture* getSpriteSheet() const;
    const sf::IntRect& getFrame() const;
    void next();
    void addFrame(sf::IntRect rect);

private:
    const sf::Texture* spriteSheet_;
    std::vector<sf::IntRect> textureRectangles_;
    std::size_t currentFrame_;
};


#endif //TERRARIA_CLONE_ANIMATION_H
