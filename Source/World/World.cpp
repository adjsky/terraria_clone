//
// Created by adjsky on 12/29/20.
//

#include "World.h"

#include <iostream>

std::unordered_map<int, Chunk> World::chunks_{};

void World::initialize() {
    BlockDatabase::initialize();
    FastNoiseLite noise_;
    noise_.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise_.SetSeed(122145);
    noise_.SetFractalOctaves(5);
    noise_.SetFrequency(0.003f);
    for (int x = 0; x < 50; x++) {
        chunks_[x * CHUNK_WIDTH].setPosition(x * CHUNK_WIDTH);
        chunks_[x * CHUNK_WIDTH].generate(noise_);
    }
}

void World::draw(sf::RenderWindow& window)  {
    const sf::View& view{ window.getView() };
    float viewStart{  (view.getCenter().x - view.getSize().x / 2.0f) / BLOCK_SIZE };
    float viewEnd{ (view.getCenter().x + view.getSize().x / 2.0f) / BLOCK_SIZE };
    for (const auto& pair : chunks_) {
        if ((pair.first <= viewEnd && pair.first >= viewStart) ||
            (pair.first + CHUNK_WIDTH <= viewEnd && pair.first + CHUNK_WIDTH>= viewStart))
        {
            pair.second.draw(window);
        }
    }
}

const Block* World::destroyBlock(sf::Vector2i pos) {
    return destroyBlock(pos.x, pos.y);
}

const Block* World::destroyBlock(int x, int y) {
    Block* block = getBlock(x, y);
    if (block) {
        if (block->visible) {
            block->visible = false;
            return block;
        }
    }
    return nullptr;
}

void World::placeBlock(sf::Vector2i pos, BlockType::Type type) {
    placeBlock(pos.x, pos.y, type);
}

void World::placeBlock(int x, int y, BlockType::Type type) {
    Block* block = getBlock(x, y);
    if (block) {
        if (!block->visible) {
            block->visible = true;
            block->type = type;
            block->sprite.setTextureRect(BlockDatabase::getData(block->type).textureRect);
        }
    }
}

Block* World::getBlock(sf::Vector2i pos) {
    return getBlock(pos.x, pos.y);
}

Block* World::getBlock(int x, int y) {
    if (x >= 0 && y >= 0 && y < CHUNK_HEIGHT) {
        int chunkPosition = x / CHUNK_WIDTH;
        int blockPositionInChunkX = x % CHUNK_WIDTH;
        int blockPositionInChunkY = y % CHUNK_HEIGHT;
        auto chunk = chunks_.find(chunkPosition * CHUNK_WIDTH);
        if (chunk != chunks_.end()) {
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