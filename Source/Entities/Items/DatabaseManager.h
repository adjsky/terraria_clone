//
// Created by adjsky on 10.02.2021.
//

#ifndef TERRARIA_CLONE_DATABASEMANAGER_H
#define TERRARIA_CLONE_DATABASEMANAGER_H

#include "Databases/BlockDatabase.h"
#include "Databases/SwordDatabase.h"

class DatabaseManager
{
public:
    DatabaseManager();

    BlockDatabase* getBlockDatabase();
    SwordDatabase* getSwordDatabase();

private:
    std::unique_ptr<BlockDatabase> blockDatabase_;
    std::unique_ptr<SwordDatabase> swordDatabase_;
};


#endif //TERRARIA_CLONE_DATABASEMANAGER_H
