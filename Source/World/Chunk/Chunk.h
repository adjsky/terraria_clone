//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_CHUNK_H
#define TERRARIA_CLONE_CHUNK_H

#include <array>
#include <memory>
#include <SFML/Graphics.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/unique_ptr.hpp>

#include "../../Entities/Items/Block/Block.h"
#include "../../Util/constants.h"
#include "../../Util/FastNoiseLite.h"

class Chunk {
public:
    explicit Chunk(int startingPosition = 0);
    Chunk(const Chunk& another_chunk);
    Chunk& operator=(const Chunk& another_chunk);
    void updateSprites();
    void generate(FastNoiseLite& noise);
    void setPosition(int x);
    void draw(sf::RenderWindow& window) const;
    Block* getBlock(int x, int y) const;

    // serialization
    friend boost::serialization::access;
    template<class Archive>
    inline void serialize(Archive & ar, const unsigned int version) {
        ar & startingPosition_;
        ar & blocks_;
    }
private:
    int startingPosition_; // x position
    std::array<std::array<std::unique_ptr<Block>, CHUNK_WIDTH>, CHUNK_HEIGHT> blocks_;
};

#endif //TERRARIA_CLONE_CHUNK_H
