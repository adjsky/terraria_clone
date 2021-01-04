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

bool checkFootBlocks(Player& player) {
    sf::FloatRect hitbox = player.getHitBox();
    for (int x = hitbox.left; x < hitbox.left + hitbox.width; x++) {
        auto block = World::getBlock(mapGlobalCoordsToGame(x, hitbox.top + hitbox.height + 0.1f));
        if (block) {
            if (block->visible) {
                return true;
            }
        }
    }
    return false;
}

bool checkPossibleVerticalCollision(Player& player) {
    sf::FloatRect hitbox = player.getHitBox();
    for (int x = hitbox.left; x < hitbox.left + hitbox.width; x++) {
        auto block = World::getBlock(mapGlobalCoordsToGame(x, hitbox.top + hitbox.height + player.verticalSpeed));
        if (block) {
            if (block->visible) {
                player.verticalSpeed = 0;
                player.setPosition(player.getPosition().x, block->sprite.getPosition().y - hitbox.height / 2.0f - 0.1f);
                return true;
            }
        }
        block = World::getBlock(mapGlobalCoordsToGame(x, hitbox.top + player.verticalSpeed));
        if (block) {
            if (block->visible) {
                player.verticalSpeed = 0;
                return true;
            }
        }
    }
    return false;
}

bool checkPossibleHorizontalCollision(Player& player) {
    sf::FloatRect hitbox = player.getHitBox();
    for (int y = hitbox.top; y < hitbox.top + hitbox.height; y++) {
        auto block = World::getBlock(mapGlobalCoordsToGame(hitbox.left + player.horizontalSpeed, y));
        if (block) {
            if (block->visible) {
                player.setPosition(block->sprite.getPosition().x + BLOCK_SIZE + hitbox.width / 2.0f + 0.1f, player.getPosition().y);
                return true;
            }
        }
        block = World::getBlock(mapGlobalCoordsToGame(hitbox.left + hitbox.width + player.horizontalSpeed, y));
        if (block) {
            if (block->visible) {
                player.setPosition(block->sprite.getPosition().x - hitbox.width / 2.0f - 0.1f, player.getPosition().y);
                return true;
            }
        }
    }
    return false;
}

#endif //TERRARIA_CLONE_COLLISIONS_H
