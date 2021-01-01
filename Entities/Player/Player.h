//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_PLAYER_H
#define TERRARIA_CLONE_PLAYER_H

#include <memory>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class Player : public sf::Drawable {
public:
    explicit Player(const std::unique_ptr<sf::Texture>& texture);

    void move(sf::Vector2f offset);
    void move(float x, float y);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Vector2f getPosition() const;
    sf::FloatRect getHitBox() const;
    void setScale(const sf::Vector2f& scale);
    sf::Vector2f getScale();
    void constructHitBox();
    void updateHitBox();
    sf::Vector2f getPosition();
    float getWidth();
    float getHeight();
    void setTextureRect(const sf::IntRect& rec);
    sf::FloatRect getGlobalBounds();

public:
    bool isOnGround;
    float verticalSpeed;
    float horizontalSpeed;

protected:
    sf::RectangleShape hitBox_;
    sf::Sprite player_;
    sf::Vector2f scale_;
};


#endif //TERRARIA_CLONE_PLAYER_H
