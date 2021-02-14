//
// Created by adjsky on 10.02.2021.
//

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() :
    blockDatabase_{},
    swordDatabase_{}
{
    blockDatabase_ = std::make_unique<BlockDatabase>();
    swordDatabase_ = std::make_unique<SwordDatabase>();
}

BlockDatabase* DatabaseManager::getBlockDatabase() {
    return blockDatabase_.get();
}

SwordDatabase* DatabaseManager::getSwordDatabase() {
    return swordDatabase_.get();
}
