//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_PLAYER_H
#define TERRARIA_CLONE_PLAYER_H

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class Player : public sf::Drawable {
private:
    sf::RectangleShape hitBox;
    sf::Sprite player;
    sf::Vector2f scale;
public:
    explicit Player(const sf::Texture& texture, const sf::IntRect& rectangle);
    void move(sf::Vector2f offset);
    void move(float x, float y);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::Vector2f getPosition() const;
    sf::FloatRect getHitBox() const;
public:
    bool isOnGround;
    float verticalSpeed;
    float horizontalSpeed;
};


#endif //TERRARIA_CLONE_PLAYER_H
