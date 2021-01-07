//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_BLOCK_H
#define TERRARIA_CLONE_BLOCK_H

#include <SFML/Graphics/Sprite.hpp>

#include "BlockInfo.h"

struct Block {
    sf::Sprite sprite;
    BlockInfo info;
    bool visible;
};

#endif //TERRARIA_CLONE_BLOCK_H