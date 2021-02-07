//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_BLOCK_H
#define TERRARIA_CLONE_BLOCK_H

#include <SFML/Graphics/Sprite.hpp>
#include <boost/serialization/serialization.hpp>

#include "BlockTypes.h"

struct Block {
    sf::Sprite sprite;
    BlockType::Type type;
    bool visible;

    // serialization
    friend boost::serialization::access;
    template<class Archive>
    inline void serialize(Archive & ar, const unsigned int version) {
        ar & type;
        ar & visible;
    }
};

#endif //TERRARIA_CLONE_BLOCK_H