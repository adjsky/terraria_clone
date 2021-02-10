//
// Created by adjsky on 1/12/21.
//

#include <algorithm>

#include "BlockDatabase.h"
#include "../../../Util/constants.h"


BlockDatabase::BlockDatabase() :
    blocksData_{}
{
    std::generate(blocksData_.begin(), blocksData_.end(), []() { return std::make_unique<BlockData>(); });
    for (std::size_t i = 0; i < blocksData_.size(); i++) {
        blocksData_[i]->textureRect.width = 64;
        blocksData_[i]->textureRect.height = 64;
        blocksData_[i]->textureRect.left = 64 * i;
    }
}

const BlockData& BlockDatabase::getData(BlockType::Type type) {
    return *blocksData_[type];
}
