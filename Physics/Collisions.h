//
// Created by adjsky on 12/30/20.
//

#ifndef TERRARIA_CLONE_COLLISIONS_H
#define TERRARIA_CLONE_COLLISIONS_H

#include "../Entities/Player/Player.h"
#include "../World/World.h"
#include "../Util/utility.h"

#include <iostream>

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
    sf::Vector2f topRightPos { hitbox.left + hitbox.width, hitbox.top};
    sf::Vector2f bottomRightPos { hitbox.left + hitbox.width, hitbox.top + hitbox.height - 1.0f};
    auto topRightBlock = World::getBlock(mapGlobalCoordsToGame(topRightPos));
    if (topRightBlock) {
        if (topRightBlock->visible) {
            return true;
        }
    }
    auto bottomRightBlock = World::getBlock(mapGlobalCoordsToGame(bottomRightPos));
    if (bottomRightBlock) {
        if (bottomRightBlock->visible) {
            return true;
        }
    }
    return false;
}
bool checkLeftSideCollision(Player& player) {
    sf::FloatRect hitbox = player.getHitBox();
    sf::Vector2f topLeftPos { hitbox.left, hitbox.top};
    sf::Vector2f bottomLeftPos { hitbox.left, hitbox.top + hitbox.height - 1.0f};
    auto topLeftBlock = World::getBlock(mapGlobalCoordsToGame(topLeftPos));
    if (topLeftBlock) {
        if (topLeftBlock->visible) {
            return true;
        }
    }
    auto bottomLeftBlock = World::getBlock(mapGlobalCoordsToGame(bottomLeftPos));
    if (bottomLeftBlock) {
        if (bottomLeftBlock->visible) {
            return true;
        }
    }
    return false;
}

#endif //TERRARIA_CLONE_COLLISIONS_H
