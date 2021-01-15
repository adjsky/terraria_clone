//
// Created by adjsky on 1/12/21.
//

#include <algorithm>

#include "BlockDatabase.h"

std::array<std::unique_ptr<BlockData>, BlockType::TYPE_COUNT> BlockDatabase::blocksData{};

void BlockDatabase::initialize() {
    std::generate(blocksData.begin(), blocksData.end(), []() { return std::make_unique<BlockData>(); });
    for (std::size_t i = 0; i < blocksData.size(); i++) {
        blocksData[i]->textureRect.width = 96;
        blocksData[i]->textureRect.height = 96;
        blocksData[i]->textureRect.left = 96 * i;
    }
}

const BlockData& BlockDatabase::getData(BlockType::Type type) {
    return *blocksData[type];
}
