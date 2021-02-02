//
// Created by adjsky on 12/30/20.
//

#include "../World/World.h"
#include "../Entities/Player/Player.h"
#include "../Util/utility.h"
#include "../Maths/Math.h"

#include <iostream>

#ifndef TERRARIA_CLONE_COLLISIONS_H
#define TERRARIA_CLONE_COLLISIONS_H

bool canPlaceBlock(const Player& player, const sf::Vector2i& pos, const World& world) {
    sf::FloatRect playerHitBox { player.getHitBox().getGlobalBounds() };
    sf::Vector2f startPos { mapGlobalCoordsToGame(playerHitBox.left, playerHitBox.top + playerHitBox.height) };
    sf::Vector2f endPos { mapGlobalCoordsToGame(playerHitBox.left + playerHitBox.width, playerHitBox.top) };
    const auto* block{ world.getBlock(pos.x, pos.y) };
    if (block) {
        if (block->visible) return false;
    }
    if (pos.x >= startPos.x && pos.x <= endPos.x && pos.y >= startPos.y && pos.y <= endPos.y) {
        return false;
    } else {
        const auto* leftBlock { world.getBlock(pos.x - 1, pos.y) };
        if (leftBlock) {
            if (leftBlock->visible) return true;
        }
        const auto* rightBlock { world.getBlock(pos.x + 1, pos.y) };
        if (rightBlock) {
            if (rightBlock->visible) return true;
        }
        const auto* topBlock { world.getBlock(pos.x, pos.y + 1) };
        if (topBlock) {
            if (topBlock->visible) return true;
        }
        const auto* bottomBlock { world.getBlock(pos.x, pos.y - 1) };
        if (bottomBlock) {
            if (bottomBlock->visible) return true;
        }

        return false;
    }
}

#endif //TERRARIA_CLONE_COLLISIONS_H