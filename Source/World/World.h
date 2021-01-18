//
// Created by adjsky on 12/29/20.
//

#ifndef TERRARIA_CLONE_WORLD_H
#define TERRARIA_CLONE_WORLD_H

#include <unordered_map>
#include <memory>

#include "Chunk/Chunk.h"
#include "../Util/utility.h"
#include "../Util/FastNoiseLite.h"
#include "Block/BlockDatabase.h"

class World {
public:
    World();
    void draw(sf::RenderWindow& window);
    const Block* destroyBlock(sf::Vector2i pos);
    const Block* destroyBlock(int x, int y);
    void placeBlock(sf::Vector2i pos, BlockType::Type type);
    void placeBlock(int x, int y, BlockType::Type type);
    Block* getBlock(sf::Vector2i pos) const;
    Block* getBlock(int x, int y) const;

private:
    // pair of x starting coordinate and chunk itself
    std::unordered_map<int, Chunk> chunks_;
};

#endif //TERRARIA_CLONE_WORLD_H