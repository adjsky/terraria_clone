//
// Created by adjsky on 12/29/20.
//

#ifndef TERRARIA_CLONE_WORLD_H
#define TERRARIA_CLONE_WORLD_H

#include <unordered_map>
#include <SFML/Graphics/Drawable.hpp>

#include "Chunk/Chunk.h"
#include "../Util/utility.h"

class World : public sf::Drawable {
public:
    explicit World();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void destroyBlock(sf::Vector2i pos);
    void destroyBlock(int x, int y);
private:
    // pair of x starting coordinate and chunk itself
    std::unordered_map<int, Chunk> chunks;
};


#endif //TERRARIA_CLONE_WORLD_H
