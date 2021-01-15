//
// Created by adjsky on 1/12/21.
//

#include <iostream>

#include "Inventory.h"

Inventory::Inventory(const sf::Vector2i &size) :
    size_{ size },
    cells_{ }
{
    cells_.resize(size.x * size.y);
}

const InventoryCell& Inventory::getCell(int x, int y) const {
    return cells_[size_.x * y + x];
}

void Inventory::addItem(BlockType::Type type) {
    std::size_t lastEmptyCell = cells_.size();
    bool pushed = false;
    for (std::size_t i = 0; i < cells_.size(); i++) {
        if (cells_[i].amount == 0) {
            if (i < lastEmptyCell) {
                lastEmptyCell = i;
            }
        }
        else if (cells_[i].blockType == type ) {
            cells_[i].amount += 1;
            pushed = true;
            break;
        }
    }
    if (!pushed) {
        cells_[lastEmptyCell].amount = 1;
        cells_[lastEmptyCell].blockType = type;
    }
}

void Inventory::removeItem(int x, int y, int amount) {
    cells_[size_.x * y + x].amount -= amount;
}

const sf::Vector2i& Inventory::getSize() const {
    return size_;
}