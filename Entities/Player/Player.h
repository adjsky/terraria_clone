//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_PLAYER_H
#define TERRARIA_CLONE_PLAYER_H

#include <memory>
#include <array>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

constexpr int PLAYER_WIDTH = 74;
constexpr int PLAYER_HEIGHT = 106;

class Player : public sf::Drawable {
public:
    explicit Player(const std::unique_ptr<sf::Texture>& texture);

    void move(sf::Vector2f offset);
    void move(float x, float y);
    void moveWithCollide();
    float getDistanceToGround() const;

    sf::Vector2f getPosition() const;
    sf::FloatRect getHitboxBounds() const;
    void constructHitBox();
    sf::Vector2f getPosition();
    void setPosition(float x, float y);
    void setTextureRect(const sf::IntRect& rec);
    void setScale(float x, float y);
    void drawHitbox(bool draw);
    bool hitboxIsDrawn();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    enum MoveDirection {
        RIGHT,
        LEFT,
        JUMP,
        DIRECTION_COUNT
    };
    bool isOnGround;
    float verticalSpeed;
    float horizontalSpeed;

protected:
    sf::RectangleShape hitBox_;
    sf::Sprite player_;

private:
    bool drawHitbox_;
};


#endif //TERRARIA_CLONE_PLAYER_H
