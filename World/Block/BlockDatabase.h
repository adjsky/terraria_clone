//
// Created by adjsky on 1/12/21.
//

#ifndef TERRARIA_CLONE_BLOCKDATABASE_H
#define TERRARIA_CLONE_BLOCKDATABASE_H

#include <array>
#include <memory>
#include <SFML/Graphics/Rect.hpp>

#include "BlockTypes.h"

struct BlockData {
    sf::IntRect textureRect;
};

class BlockDatabase {
public:
    static void initialize();
    static const BlockData& getData(BlockType::Type type);
private:
    static std::array<std::unique_ptr<BlockData>, BlockType::TYPE_COUNT> blocksData;
};


#endif //TERRARIA_CLONE_BLOCKDATABASE_H