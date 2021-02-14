//
// Created by adjsky on 1/12/21.
//

#ifndef TERRARIA_CLONE_INVENTORY_H
#define TERRARIA_CLONE_INVENTORY_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#include "../Items/ItemTypes.h"

class Inventory {
public:
    struct Cell {
        std::size_t amount = 0;
        ItemTypes itemType;
        int id;

        // serialization
        friend boost::serialization::access;
        template<class Archive>
        inline void serialize(Archive & ar, const unsigned int version) {
            ar & amount;
            ar & id;
            ar & itemType;
        }
    };
public:
    Inventory();
    explicit Inventory(const sf::Vector2i& size);
    const Cell& getCell(int x, int y) const;
    Cell& getCell(int x, int y);
    bool addItem(const Cell& cell);
    bool setItem(const Cell& cell, int x, int y);
    void removeItem(int x, int y, int amount = -1);
    void setSize(const sf::Vector2i& size);
    const sf::Vector2i& getSize() const;

    // serialization
    friend boost::serialization::access;
    template<class Archive>
    inline void serialize(Archive & ar, const unsigned int version) {
        ar & size_;
        ar & cells_;
    }
private:
    sf::Vector2i size_;
    std::vector<std::vector<Cell>> cells_;
};

#endif //TERRARIA_CLONE_INVENTORY_H