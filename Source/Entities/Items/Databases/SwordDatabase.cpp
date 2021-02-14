//
// Created by adjsky on 12.02.2021.
//

#include "SwordDatabase.h"

SwordDatabase::SwordDatabase()
    : swordsData_{  }
{
    std::generate(swordsData_.begin(), swordsData_.end(), []() { return std::make_unique<SwordData>(); });
//    for (std::size_t i = 0; i < swordsData_.size(); i++) {
//        swordsData_[i]->textureRect.left = 22 * i;
//        swordsData_[i]->textureRect.width = 22;
//        swordsData_[i]->textureRect.height = 22;
//    }
}

SwordData* SwordDatabase::getData(SwordType type) {
    return swordsData_[static_cast<std::size_t>(type)].get();
}
