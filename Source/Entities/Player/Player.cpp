//
// Created by kirill on 08.01.2021.
//

#include "Player.h"
#include "../../Util/Serialization/GameSerialization.h"

Player::Player() :
    AnimatedSprite{ },
    isOnGround{ },
    verticalSpeed{ },
    horizontalSpeed{ },
    hasAttachedItem{ false },
    attachedItem{ },
    hitBox_{ },
    animations_{ },
    hotBar_{ sf::Vector2i{ PLAYER_HOTBAR_SIZE, 1 }},
    backpack_{ sf::Vector2i{ PLAYER_BACKPACK_SIZE.x, PLAYER_BACKPACK_SIZE.y }},
    hotBarIndex_{ 0 },
    health_{ 100 }
{
}

void Player::moveWithCollide(const World& world)
{
    if (horizontalSpeed != 0) {
        sf::FloatRect hitbox = getHitBox().getGlobalBounds();
        // move hitbox horizontally
        hitbox.left += horizontalSpeed;

        // left bottom
        sf::Vector2i startPos = mapGlobalCoordsToGame(hitbox.left, hitbox.top + hitbox.height);
        // top right
        sf::Vector2i endPos = mapGlobalCoordsToGame(hitbox.left + hitbox.width, hitbox.top);

        // right block
        if (horizontalSpeed > 0) {
            float diff = std::numeric_limits<float>::max();
            for (int y = startPos.y; y < endPos.y + 1; y++) {
                const auto* block = world.getBlock(endPos.x, y);
                if (block) {
                    if (block->visible) {
                        if (hitbox.intersects(block->sprite.getGlobalBounds())) {
                            diff = block->sprite.getGlobalBounds().left - (getHitBox().getGlobalBounds().left + getHitBox().getGlobalBounds().width);
                            break;
                        }
                    }
                }
            }
            if (diff < horizontalSpeed) {
                move(diff - 0.01f, 0); // subtract 0.01 to prevent stuck in a block
            }
            else {
                move(horizontalSpeed, 0);
            }
        }

        // left block
        if (horizontalSpeed < 0) {
            float diff = std::numeric_limits<float>::max();
            for (int y = startPos.y; y < endPos.y + 1; y++) {
                const auto* block = world.getBlock(startPos.x, y);
                if (block) {
                    if (block->visible) {
                        if (hitbox.intersects(block->sprite.getGlobalBounds())) {
                            diff = (block->sprite.getGlobalBounds().left + block->sprite.getGlobalBounds().width) -
                                getHitBox().getGlobalBounds().left;
                            break;
                        }
                    }
                }
            }
            if (diff > horizontalSpeed && diff != std::numeric_limits<float>::max()) {
                move(diff + 0.01f, 0); // add 0.01 to prevent stuck in a block
            }
            else {
                move(horizontalSpeed, 0);
            }
        }
    }

    // block above
    if (verticalSpeed < 0) {
        sf::FloatRect hitbox = getHitBox().getGlobalBounds();
        // move hitbox vertically
        hitbox.top += verticalSpeed;

        // left bottom
        sf::Vector2i startPos = mapGlobalCoordsToGame(hitbox.left, hitbox.top + hitbox.height);
        // top right
        sf::Vector2i endPos = mapGlobalCoordsToGame(hitbox.left + hitbox.width, hitbox.top);

        for (int x = startPos.x; x < endPos.x + 1; x++) {
            const auto* block = world.getBlock(x, endPos.y);
            if (block) {
                if (block->visible) {
                    if (hitbox.intersects(block->sprite.getGlobalBounds())) {
                        verticalSpeed = 0;
                    }
                    break;
                }
            }
        }
        move(0, verticalSpeed);
    }

    // block under
    if (verticalSpeed > 0) {
        float diff = getDistanceToGround(world);
        if (diff < verticalSpeed) {
            move(0, diff - 0.01f); // subtract 0.01 to prevent stuck in a block
            verticalSpeed = 0;
        }
        else {
            move(0, verticalSpeed);
        }
    }
}

float Player::getDistanceToGround(const World& world) const
{
    sf::FloatRect hitbox = getHitBox().getGlobalBounds();
    float minHeight = std::numeric_limits<float>::max();
    int startX = mapGlobalCoordsToGame(hitbox.left, 0).x;
    int endX = mapGlobalCoordsToGame(hitbox.left + hitbox.width, 0).x;
    sf::Vector2i playerCoords = mapGlobalCoordsToGame(getPosition().x, hitbox.top + hitbox.height);
    for (int x = startX; x < endX + 1; x++) {
        const Block* block{ nullptr };
        for (int y = 0; y < playerCoords.y; y++) {
            block = world.getBlock(x, playerCoords.y - y);
            if (block) {
                if (block->visible) {
                    float height = -((hitbox.top + hitbox.height) - block->sprite.getPosition().y);
                    if (height < minHeight) {
                        minHeight = height;
                    }
                    break;
                }
            }
        }
    }
    return minHeight;
}

void Player::constructHitBox()
{
    hitBox_.setOutlineThickness(1.0f);
    hitBox_.setOutlineColor(sf::Color::Red);
    hitBox_.setFillColor(sf::Color::Transparent);
    hitBox_.setSize(sf::Vector2f(BLOCK_SIZE - (BLOCK_SIZE / 5), BLOCK_SIZE * 3 - BLOCK_SIZE / 5));
    hitBox_.setOrigin(hitBox_.getSize().x / 2.0f, hitBox_.getSize().y / 2.0f);
}

const sf::RectangleShape& Player::getHitBox() const
{
    return hitBox_;
}

void Player::move(float x, float y)
{
    AnimatedSprite::move(x, y);
    hitBox_.move(x, y);
}

Animation& Player::getAnimation(Player::AnimationTypes type)
{
    return animations_[type];
}

Inventory& Player::getHotBar()
{
    return hotBar_;
}

Inventory& Player::getBackpack()
{
    return backpack_;
}

const Inventory& Player::getHotBar() const
{
    return hotBar_;
}

const Inventory& Player::getBackpack() const
{
    return backpack_;
}

int Player::getHotBarIndex() const
{
    return hotBarIndex_;
}

void Player::setHotBarIndex(int i)
{
    hotBarIndex_ = i;
}

int Player::getHealth() const
{
    return health_;
}
