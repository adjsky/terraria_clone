#include "DatabaseManager.h"

template<class T>
T* DatabaseManager::getDatabase() {
    if (std::is_same<BlockDatabase, T>::value) {
        return blockDatabase_.get();
    }
}