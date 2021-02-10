//
// Created by adjsky on 10.02.2021.
//

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() :
    blockDatabase_{}
{
    blockDatabase_ = std::make_unique<BlockDatabase>();
}