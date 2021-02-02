//
// Created by adjsky on 1/12/21.
//

#ifndef TERRARIA_CLONE_INVENTORY_H
#define TERRARIA_CLONE_INVENTORY_H

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "../../World/Block/BlockTypes.h"

class Inventory {
public:
    struct Cell {
        std::size_t amount = 0;
        BlockType::Type blockType = BlockType::AIR;
    };
public:
    explicit Inventory(const sf::Vector2i& size);
    const Cell& getCell(int x, int y) const;
    Cell& getCell(int x, int y);
    bool addItem(BlockType::Type type, int amount);
    bool setItem(const Cell& cell, int x, int y);
    void removeItem(int x, int y, int amount = -1);
    const sf::Vector2i& getSize() const;

private:
    sf::Vector2i size_;
    std::vector<std::vector<Cell>> cells_;
};

#endif //TERRARIA_CLONE_INVENTORY_H