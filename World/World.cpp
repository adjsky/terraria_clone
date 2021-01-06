//
// Created by adjsky on 12/29/20.
//

#include "World.h"
#include "../ResourceManager/ResourceManager.h"
#include <iostream>

std::unordered_map<int, Chunk> World::chunks_{  };

void World::initialize() {
    FastNoiseLite noise_;
    noise_.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise_.SetSeed(1223311145);
    noise_.SetFractalOctaves(5);
    noise_.SetFrequency(0.003f);
    for (int x = 0; x < 15; x++) {
        chunks_[x * CHUNK_WIDTH] = Chunk{x * CHUNK_WIDTH, noise_ };
    }
}

void World::draw(sf::RenderWindow& window)  {
    for (const auto& pair : chunks_) {
        pair.second.draw(window);
    }
}

void World::destroyBlock(sf::Vector2i pos) {
    destroyBlock(pos.x, pos.y);
}

void World::destroyBlock(int x, int y) {
    std::shared_ptr<Block> block = getBlock(x, y);
    if (block) {
        getBlock(x, y)->visible = false;
    }
}

std::shared_ptr<Block> World::getBlock(sf::Vector2i pos) {
    return getBlock(pos.x, pos.y);
}

std::shared_ptr<Block> World::getBlock(int x, int y) {
    if (x >= 0 && y >= 0 && y < CHUNK_HEIGHT) {
        int chunkPosition = x / CHUNK_WIDTH;
        int blockPositionInChunkX = x % CHUNK_WIDTH;
        int blockPositionInChunkY = y % CHUNK_HEIGHT;
        if (chunkPosition < chunks_.size()) {
            return chunks_.at(chunkPosition * CHUNK_WIDTH).getBlock(blockPositionInChunkX, blockPositionInChunkY);
        }
        else {
            return nullptr;
        }
    }
    else {
        return nullptr;
    }
}

void World::placeBlock(sf::Vector2i pos) {
    placeBlock(pos.x, pos.y);
}

void World::placeBlock(int x, int y) {
    std::shared_ptr<Block> block = getBlock(x, y);
    if (block) {
        if (!block->visible) {
            block->visible = true;
            block->info.type = BlockType::GRASS;
            block->sprite.setTextureRect(sf::IntRect(0, 0, 96, 96));
        }
    }
}

