//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_CHUNK_H
#define TERRARIA_CLONE_CHUNK_H

#include <array>
#include <SFML/Graphics.hpp>
#include <memory>

#include "../Block/Block.h"
#include "../../Util/constants.h"

class Chunk : public sf::Drawable {
public:
    explicit Chunk() = default;
    explicit Chunk(sf::Vector2i pos);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void destroyBlock(sf::Vector2i pos);
    void destroyBlock(int x, int y);
private:
    sf::Vector2i position;
    std::array<std::array<Block, CHUNK_WIDTH>, CHUNK_HEIGHT> blocks;
};


#endif //TERRARIA_CLONE_CHUNK_H
