//
// Created by adjsky on 1/12/21.
//

#include <algorithm>

#include "BlockDatabase.h"
#include "../../../Util/constants.h"


BlockDatabase::BlockDatabase() :
    blocksData_{ }
{
    std::generate(blocksData_.begin(), blocksData_.end(), []()
    { return std::make_unique<BlockData>(); });
}

const BlockData* BlockDatabase::getData(BlockType type)
{
    return blocksData_[static_cast<std::size_t>(type)].get();
}
