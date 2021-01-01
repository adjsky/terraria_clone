//
// Created by adjsky on 12/30/20.
//

#ifndef TERRARIA_CLONE_COLLISIONS_H
#define TERRARIA_CLONE_COLLISIONS_H

#include <array>
#include <iostream>

#include "../Entities/Player/Player.h"
#include "../World/World.h"
#include "../Util/utility.h"

bool checkBottomCollision(const Player& player) {
    sf::FloatRect hitbox = player.getHitBox();
    sf::Vector2f bottomLeftPos { hitbox.left, hitbox.top + hitbox.height + player.verticalSpeed};
    auto leftBottomBlock = World::getBlock(mapGlobalCoordsToGame(bottomLeftPos));
    if (leftBottomBlock) {
        if (leftBottomBlock->visible) {
            return true;
        }
    }
    sf::Vector2f bottomRightPos { hitbox.left + hitbox.width, hitbox.top + hitbox.height + player.verticalSpeed};
    auto rightBottomBlock = World::getBlock(mapGlobalCoordsToGame(bottomRightPos));
    if (rightBottomBlock) {
        if (rightBottomBlock->visible) {
            return true;
        }
    }
    return false;
}

bool checkRightSideCollision(const Player& player) {
    sf::FloatRect hitbox = player.getHitBox();
    std::array<sf::Vector2f, 3> positions;
    positions[0] = sf::Vector2f(hitbox.left + hitbox.width, hitbox.top + hitbox.height - 0.1f);
    positions[1] = sf::Vector2f(hitbox.left + hitbox.width, hitbox.top + 0.1f);
    positions[2] = sf::Vector2f(hitbox.left + hitbox.width, hitbox.top + hitbox.height / 2.0f);
    for (auto& pos : positions) {
        auto block = World::getBlock(mapGlobalCoordsToGame(pos));
        if (block) {
            if (block->visible) {
                if (hitbox.intersects(block->sprite.getGlobalBounds())) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkLeftSideCollision(Player& player) {
    sf::FloatRect hitbox = player.getHitBox();
    std::array<sf::Vector2f, 3> positions;
    positions[0] = sf::Vector2f(hitbox.left, hitbox.top + hitbox.height - 0.1f);
    positions[1] = sf::Vector2f(hitbox.left, hitbox.top + 0.1f);
    positions[2] = sf::Vector2f(hitbox.left, hitbox.top + hitbox.height / 2.0f);
    for (auto& pos : positions) {
        auto block = World::getBlock(mapGlobalCoordsToGame(pos));
        if (block) {
            if (block->visible) {
                if (hitbox.intersects(block->sprite.getGlobalBounds())) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkTopCollision(Player& player) {
    sf::FloatRect hitbox = player.getHitBox();
    sf::Vector2f bottomLeftPos { hitbox.left, hitbox.top + player.verticalSpeed};
    auto leftTopBlock = World::getBlock(mapGlobalCoordsToGame(bottomLeftPos));
    if (leftTopBlock) {
        if (leftTopBlock->visible) {
            return true;
        }
    }
    sf::Vector2f TopRightPos { hitbox.left + hitbox.width, hitbox.top + player.verticalSpeed};
    auto rightTopBlock = World::getBlock(mapGlobalCoordsToGame(TopRightPos));
    if (rightTopBlock) {
        if (rightTopBlock->visible) {
            return true;
        }
    }
    return false;
}

#endif //TERRARIA_CLONE_COLLISIONS_H
