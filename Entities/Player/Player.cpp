//
// Created by kirill on 08.01.2021.
//

#include "Player.h"
#include "../../Util/utility.h"
#include "../../World/World.h"
#include "../../InputHandler/InputHandler.h"

Player::Player() :
    AnimatedSprite{},
    isOnGround{ },
    verticalSpeed{ },
    horizontalSpeed{ },
    hitBox_{ },
    animations_{ },
    hotBar_{ sf::Vector2i{ 10, 1 } },
    heldItem_{ 0 }
{

}

void Player::moveWithCollide() {
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
                Block* block = World::getBlock(endPos.x, y);
                if (block) {
                    if (block->visible) {
                        if (hitbox.intersects(block->sprite.getGlobalBounds())) {
                            diff = block->sprite.getGlobalBounds().left - (getHitBox().getGlobalBounds().left + getHitBox().getGlobalBounds().width) ;
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
                Block* block = World::getBlock(startPos.x, y);
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
            Block* block = World::getBlock(x, endPos.y);
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
        float diff = getDistanceToGround();
        if (diff < verticalSpeed) {
            move(0, diff - 0.01f); // subtract 0.01 to prevent stuck in a block
            verticalSpeed = 0;
        } else {
            move(0, verticalSpeed);
        }
    }
}

float Player::getDistanceToGround() const {
    sf::FloatRect hitbox = getHitBox().getGlobalBounds();
    float minHeight = std::numeric_limits<float>::max();
    int startX = mapGlobalCoordsToGame(hitbox.left, 0).x;
    int endX = mapGlobalCoordsToGame(hitbox.left + hitbox.width, 0).x;
    sf::Vector2i playerCoords = mapGlobalCoordsToGame(getPosition().x, hitbox.top + hitbox.height);
    for (int x = startX; x < endX + 1; x++) {
        Block* block;
        for (int y = 0; y < playerCoords.y; y++) {
            block = World::getBlock(x, playerCoords.y - y);
            if (block) {
                if (block->visible) {
                    float height =  -((hitbox.top + hitbox.height) - block->sprite.getPosition().y);
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

void Player::constructHitBox() {
    hitBox_.setOutlineThickness(1.5f);
    hitBox_.setOutlineColor(sf::Color::Blue);
    hitBox_.setFillColor(sf::Color::Transparent);
    hitBox_.setSize(sf::Vector2f(BLOCK_SIZE - (BLOCK_SIZE / 5.0f), BLOCK_SIZE * 3.0f - BLOCK_SIZE / 5.0f));
    hitBox_.setOrigin(hitBox_.getSize().x / 2.0f, hitBox_.getSize().y / 2.0f);
}

const sf::RectangleShape &Player::getHitBox() const{
    return hitBox_;
}

void Player::move(float x, float y) {
    AnimatedSprite::move(x, y);
    hitBox_.move(x, y);
}

Animation& Player::getAnimation(Player::AnimationTypes type) {
    return animations_[type];
}

void Player::updateStates() {
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num1) == InputHandler::JUST_PRESSED) {
        heldItem_ = 0;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num2) == InputHandler::JUST_PRESSED) {
        heldItem_ = 1;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num3) == InputHandler::JUST_PRESSED) {
        heldItem_ = 2;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num4) == InputHandler::JUST_PRESSED) {
        heldItem_ = 3;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num5) == InputHandler::JUST_PRESSED) {
        heldItem_ = 4;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num6) == InputHandler::JUST_PRESSED) {
        heldItem_ = 5;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num7) == InputHandler::JUST_PRESSED) {
        heldItem_ = 6;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num8) == InputHandler::JUST_PRESSED) {
        heldItem_ = 7;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num9) == InputHandler::JUST_PRESSED) {
        heldItem_ = 8;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num0) == InputHandler::JUST_PRESSED) {
        heldItem_ = 9;
    }
}

Inventory& Player::getHotBar() {
    return hotBar_;
}

int Player::getHeldItem() {
    return heldItem_;
}


