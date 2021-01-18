//
// Created by adjsky on 12/28/20.
//

#include "Chunk.h"
#include "../Block/BlockDatabase.h"
#include "../../ResourceManager/ResourceManager.h"

Chunk::Chunk(int startingPosition) :
    startingPosition_{ startingPosition }
{
}

void Chunk::generate(FastNoiseLite& noise) {
    for (int x = 0; x < CHUNK_WIDTH; x++) {
        float noiseValue = noise.GetNoise((float)(x+startingPosition_), 0.0f);
        int blockHeight = WORLD_HEIGHT_GENERATION + (int)(noiseValue * 20);
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            blocks_[y][x] = std::make_unique<Block>();
            blocks_[y][x]->sprite.setTexture(ResourceManager::getTexture(ResourceManager::BLOCK));
            blocks_[y][x]->sprite.move(startingPosition_ * BLOCK_SIZE + x * BLOCK_SIZE, -y * BLOCK_SIZE);
            blocks_[y][x]->sprite.setScale(BLOCK_SIZE / 64.0f, BLOCK_SIZE / 64.0f);
            if (y > blockHeight) {
                blocks_[y][x]->type = BlockType::AIR;
                blocks_[y][x]->visible = false;
            }
            else {
                if (y == blockHeight)
                {
                    blocks_[y][x]->type = BlockType::GRASS;
                }
                else {
                    blocks_[y][x]->type = BlockType::DIRT_PIXEL;
                }
                blocks_[y][x]->visible = true;
                blocks_[y][x]->sprite.setTextureRect(BlockDatabase::getData(blocks_[y][x]->type).textureRect);
            }
        }
    }
}

void Chunk::setPosition(int x) {
    startingPosition_ = x;
}

void Chunk::draw(sf::RenderWindow& window) const {
    const sf::View& view = window.getView();
    sf::FloatRect viewBox { view.getCenter() - view.getSize() / 2.0f, view.getSize() };
    for (const auto& arr : blocks_) {
        for (const auto& block : arr) {
            if (block->visible) {
                if (viewBox.intersects(block->sprite.getGlobalBounds())){
                    window.draw(block->sprite);
                }
            }
        }
    }
}

Block* Chunk::getBlock(int x, int y) {
    return blocks_[y][x].get();
}
