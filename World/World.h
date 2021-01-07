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

class World {
public:
    static void initialize();
    static void draw(sf::RenderWindow& window);
    static void destroyBlock(sf::Vector2i pos);
    static void destroyBlock(int x, int y);
    static void placeBlock(sf::Vector2i pos);
    static void placeBlock(int x, int y);
    static Block* getBlock(sf::Vector2i pos);
    static Block* getBlock(int x, int y);

private:
    // pair of x starting coordinate and chunk itself
    static std::unordered_map<int, Chunk> chunks_;
};

#endif //TERRARIA_CLONE_WORLD_H