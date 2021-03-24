//
// Created by adjsky on 1/12/21.
//

#ifndef TERRARIA_CLONE_BLOCKDATABASE_H
#define TERRARIA_CLONE_BLOCKDATABASE_H

#include <array>
#include <memory>
#include <SFML/Graphics/Rect.hpp>

#include "../ItemTypes.h"

struct BlockData
{
};

class BlockDatabase
{
public:
    BlockDatabase();
    const BlockData* getData(BlockType type);
private:
    std::array<std::unique_ptr<BlockData>, static_cast<std::size_t>(BlockType::TYPES_COUNT)> blocksData_;
};


#endif //TERRARIA_CLONE_BLOCKDATABASE_H
