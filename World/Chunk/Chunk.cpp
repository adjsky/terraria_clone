//
// Created by adjsky on 12/28/20.
//

#include "Chunk.h"
#include "../../ResourceManager/ResourceManager.h"

#include <cmath>

#include <iostream>

Chunk::Chunk(sf::Vector2i pos) : position{ pos } {
    for (int y = 0; y < CHUNK_HEIGHT; y++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            blocks[y][x].info = BlockInfo{ BlockType::DIRT_PIXEL };
            blocks[y][x].visible = true;
            blocks[y][x].sprite = sf::Sprite(*ResourceManager::getBlockTexture(), sf::IntRect(blocks[y][x].info.type * 96, 0, 96, 96));
            blocks[y][x].sprite.move(position.x + x * BLOCK_SIZE, position.y - y * BLOCK_SIZE);
            blocks[y][x].sprite.scale(BLOCK_SIZE / 96.0f, BLOCK_SIZE / 96.0f);
        }
    }
}

void Chunk::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (const auto& arr : blocks) {
        for (const auto& block : arr) {
            if (block.visible) {
                target.draw(block.sprite);
            }
        }
    }
}

void Chunk::destroyBlock(sf::Vector2i pos) {
    destroyBlock(pos.x, pos.y);
}

void Chunk::destroyBlock(int x, int y) {
    blocks[y][x].visible = false;
}



