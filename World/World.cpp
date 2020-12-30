//
// Created by adjsky on 12/29/20.
//

#include "World.h"

std::unordered_map<int, Chunk> World::chunks{  };

//World::World() {
//    for (int x = 0; x < 5; x++) {
//        sf::Vector2i position = sf::Vector2i(x * CHUNK_WIDTH * BLOCK_SIZE, 0);
//        chunks[x * CHUNK_WIDTH] = Chunk{ position };
//    }
//}

void World::initialize() {
    for (int x = 0; x < 5; x++) {
        sf::Vector2i position = sf::Vector2i(x * CHUNK_WIDTH * BLOCK_SIZE, 0);
        chunks[x * CHUNK_WIDTH] = Chunk{ position };
    }
}

void World::draw(sf::RenderWindow& window)  {
    for (const auto& pair : chunks) {
        window.draw(pair.second);
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
        if (chunkPosition < chunks.size()) {
            return chunks.at(chunkPosition * CHUNK_WIDTH).getBlock(blockPositionInChunkX, blockPositionInChunkY);
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
        getBlock(x, y)->visible = true;
    }
}

