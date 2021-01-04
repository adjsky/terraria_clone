//
// Created by adjsky on 12/28/20.
//

#include "Player.h"
#include "../../Util/utility.h"
#include "../../World/World.h"

Player::Player(const std::unique_ptr<sf::Texture>& texture) :
    player_ { *texture },
    isOnGround{ },
    verticalSpeed{ },
    horizontalSpeed{ },
    scale_ { sf::Vector2f{ 1.0f, 1.0f } }
{
}

void Player::move(sf::Vector2f offset) {
    move(offset.x, offset.y);
}

void Player::move(float x, float y) {
    hitBox_.move(x, y);
    player_.move(x, y);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(hitBox_);
    target.draw(player_);
}

sf::Vector2f Player::getPosition() const {
    return player_.getPosition();
}

sf::FloatRect Player::getHitBox() const {
    return hitBox_.getGlobalBounds();
}

void Player::setScale(const sf::Vector2f& scale) {
    scale_ = scale;
    player_.setScale(scale_);
}

sf::Vector2f Player::getScale() {
    return scale_;
}

void Player::constructHitBox() {
    hitBox_.setOutlineThickness(1.0f);
    hitBox_.setOutlineColor(sf::Color::Blue);
    hitBox_.setFillColor(sf::Color::Transparent);
    hitBox_.setSize(sf::Vector2f(player_.getTextureRect().width * scale_.x - 5.0f, player_.getTextureRect().height * scale_.y));
    hitBox_.setOrigin(hitBox_.getSize().x / 2.0f, hitBox_.getSize().y / 2.0f);
}

void Player::updateHitBox() {
    hitBox_.setSize(sf::Vector2f(player_.getTextureRect().width * scale_.x - 5.0f, player_.getTextureRect().height * scale_.y));
    hitBox_.setOrigin(hitBox_.getSize().x / 2.0f, hitBox_.getSize().y / 2.0f);
}

void Player::setTextureRect(const sf::IntRect &rec) {
    player_.setTextureRect(rec);
    player_.setOrigin(rec.width / 2.0f, rec.height / 2.0f);
    setScale(sf::Vector2f(static_cast<float>(BLOCK_SIZE) / rec.width - 0.1f,
                                  static_cast<float>(BLOCK_SIZE) / rec.height * 2 - 0.02f));
    updateHitBox();
}

sf::Vector2f Player::getPosition() {
    return sf::Vector2f(player_.getPosition());
}

sf::FloatRect Player::getGlobalBounds() {
    return player_.getGlobalBounds();
}

void Player::setPosition(float x, float y) {
    hitBox_.setPosition(x, y);
    player_.setPosition(x, y);
}


