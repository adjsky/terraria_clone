//
// Created by adjsky on 12/29/20.
//

#include "World.h"

World::World() {
    for (int x = 0; x < 5; x++) {
        sf::Vector2i position = sf::Vector2i(x * CHUNK_WIDTH * BLOCK_SIZE, 0);
        chunks[x * CHUNK_WIDTH] = Chunk{ position };
    }

}

void World::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (const auto& pair : chunks) {
        target.draw(pair.second);
    }
}

void World::destroyBlock(sf::Vector2i pos) {
    destroyBlock(pos.x, pos.y);
}

void World::destroyBlock(int x, int y) {
    if (x >= 0 && y >= 0 && y < CHUNK_HEIGHT) {
        int chunkPosition = x / CHUNK_WIDTH;
        int blockPositionInChunkX = x % CHUNK_WIDTH;
        int blockPositionInChunkY = y % CHUNK_HEIGHT;
        if (chunkPosition < chunks.size()) {
            chunks.at(chunkPosition * CHUNK_WIDTH).destroyBlock(blockPositionInChunkX, blockPositionInChunkY);
        }
    }
}

