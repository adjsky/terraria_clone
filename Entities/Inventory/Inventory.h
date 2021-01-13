//
// Created by adjsky on 1/12/21.
//

#ifndef TERRARIA_CLONE_INVENTORY_H
#define TERRARIA_CLONE_INVENTORY_H

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "../../World/Block/BlockTypes.h"

struct InventoryCell {
    std::size_t amount;
    BlockType::Type blockType;
};

class Inventory {
public:
    explicit Inventory(const sf::Vector2i& size);
    const InventoryCell& getCell(int x, int y) const;
    void addItem(BlockType::Type type);
    void removeItem(int x, int y, int amount);
    const sf::Vector2i& getSize() const;

private:
    sf::Vector2i size_;
    std::vector<InventoryCell> cells_;
};

#endif //TERRARIA_CLONE_INVENTORY_H