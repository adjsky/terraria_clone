//
// Created by adjsky on 10.02.2021.
//

#ifndef TERRARIA_CLONE_DATABASEMANAGER_H
#define TERRARIA_CLONE_DATABASEMANAGER_H

#include "Databases/BlockDatabase.h"

class DatabaseManager {
public:
    DatabaseManager();

    template<class T>
    T* getDatabase();

private:
    std::unique_ptr<BlockDatabase> blockDatabase_;
};

#include "DatabaseManager.inl"


#endif //TERRARIA_CLONE_DATABASEMANAGER_H
