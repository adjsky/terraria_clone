//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_CHUNK_H
#define TERRARIA_CLONE_CHUNK_H

#include <SFML/Graphics.hpp>
#include <array>
#include <memory>

#include "../Block/Block.h"
#include "../../Util/constants.h"
#include "../../Util/FastNoiseLite.h"

class Chunk {
public:
    explicit Chunk(int startingPosition = 0);
    void generate(FastNoiseLite& noise);
    void setPosition(int x);
    void draw(sf::RenderWindow& window) const;
    Block* getBlock(int x, int y) const;
private:
    int startingPosition_; // x position
    std::array<std::array<std::unique_ptr<Block>, CHUNK_WIDTH>, CHUNK_HEIGHT> blocks_;
};

#endif //TERRARIA_CLONE_CHUNK_H
