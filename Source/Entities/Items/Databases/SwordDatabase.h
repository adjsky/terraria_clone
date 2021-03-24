//
// Created by adjsky on 12.02.2021.
//

#ifndef TERRARIA_CLONE_SWORDDATABASE_H
#define TERRARIA_CLONE_SWORDDATABASE_H

#include <array>
#include <memory>
#include <SFML/Graphics/Rect.hpp>
#include "../ItemTypes.h"

struct SwordData
{
};

class SwordDatabase
{
public:
    SwordDatabase();
    SwordData* getData(SwordType type);

private:
    std::array<std::unique_ptr<SwordData>, static_cast<std::size_t>(SwordType::TYPES_COUNT)> swordsData_;
};


#endif //TERRARIA_CLONE_SWORDDATABASE_H
