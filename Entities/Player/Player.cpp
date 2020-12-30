//
// Created by adjsky on 12/28/20.
//

#include "Player.h"
#include "../../Util/utility.h"
#include "../../World/World.h"
#include "../../Util/constants.h"

Player::Player(const sf::Texture& texture, const sf::IntRect& rectangle) :
    player { texture, rectangle },
    isOnGround{ },
    verticalSpeed{ },
    horizontalSpeed{ },
    scale {static_cast<float>(BLOCK_SIZE) / rectangle.width - 0.1f, static_cast<float>(BLOCK_SIZE) / rectangle.height * 2 - 0.02f }
{
    player.setOrigin(rectangle.width / 2.0f, rectangle.height / 2.0f);
    player.setScale(scale);
    hitBox.setSize(sf::Vector2f(rectangle.width * scale.x, rectangle.height * scale.y));
    hitBox.setOutlineThickness(1.0f);
    hitBox.setOutlineColor(sf::Color::Blue);
    hitBox.setFillColor(sf::Color::Transparent);
    hitBox.setOrigin(hitBox.getSize().x / 2.0f, hitBox.getSize().y / 2.0f);
}

void Player::move(sf::Vector2f offset) {
    move(offset.x, offset.y);
}

void Player::move(float x, float y) {
    hitBox.move(x, y);
    player.move(x, y);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(hitBox);
    target.draw(player);
}

sf::Vector2f Player::getPosition() const {
    return player.getPosition();
}

sf::FloatRect Player::getHitBox() const {
    return hitBox.getGlobalBounds();
}
