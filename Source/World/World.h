//
// Created by adjsky on 12/29/20.
//

#ifndef TERRARIA_CLONE_WORLD_H
#define TERRARIA_CLONE_WORLD_H

#include <iostream>
#include <unordered_map>
#include <memory>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>

#include "Chunk/Chunk.h"
#include "../Util/utility.h"
#include "../Util/FastNoiseLite.h"
#include "Block/BlockDatabase.h"

class World {
public:
    World();
    void generate();
    void updateSprites();
    void draw(sf::RenderWindow& window);
    const Block* destroyBlock(int x, int y);
    bool placeBlock(int x, int y, BlockType::Type type);
    Block* getBlock(int x, int y);
    const Block* getBlock(int x, int y) const;

    // serialization
    friend boost::serialization::access;
    template<class Archive>
    inline void serialize(Archive & ar, const unsigned int version) {
        ar & chunks_;
    }
private:
    // pair of x starting coordinate and chunk itself
    std::unordered_map<int, Chunk> chunks_;
};

#endif //TERRARIA_CLONE_WORLD_H