//
// Created by adjsky on 1/12/21.
//

#include <iostream>
#include <functional>

#include "Inventory.h"

Inventory::Inventory() :
    size_{ },
    cells_{ }
{
}

Inventory::Inventory(const sf::Vector2i& size) :
    size_{ size },
    cells_{ }
{
    cells_.resize(size.y);
    for (int y = 0; y < size.y; y++) {
        cells_[y].resize(size.x);
    }
}

const Inventory::Cell& Inventory::getCell(int x, int y) const {
    return cells_[y][x];
}

Inventory::Cell& Inventory::getCell(int x, int y) {
    return cells_[y][x];
}

bool Inventory::addItem(BlockType::Type type, int amount) {
    sf::Vector2i lastEmptyCell = size_;
    for (int y = 0; y < size_.y; y++) {
        for (int x = 0; x < size_.x; x++) {
            if (cells_[y][x].amount == 0) {
                if (y < lastEmptyCell.y && x < lastEmptyCell.x) {
                    lastEmptyCell.x = x;
                    lastEmptyCell.y = y;
                }
            }
            else if (cells_[y][x].blockType == type) {
                cells_[y][x].amount += amount;
                return true;
            }
        }
    }
    if (lastEmptyCell == size_ && cells_[lastEmptyCell.y][lastEmptyCell.x].blockType != BlockType::AIR) return false;
    cells_[lastEmptyCell.y][lastEmptyCell.x].amount = amount;
    cells_[lastEmptyCell.y][lastEmptyCell.x].blockType = type;
    return true;
}

bool Inventory::setItem(const Inventory::Cell &cell, int x, int y) {
    if (cell.blockType == cells_[y][x].blockType) {
        cells_[y][x].amount += cell.amount;
    }
    else {
        bool swapped{ false };
        if (cells_[y][x].amount > 0) {
            swapped = true;
        }
        cells_[y][x] = cell;
        return swapped;
    }
    return false;
}

void Inventory::removeItem(int x, int y, int amount) {
    if (amount != -1) {
        cells_[y][x].amount -= amount;
        if (cells_[y][x].amount <= 0) {
            cells_[y][x].amount = 0;
            cells_[y][x].blockType = BlockType::AIR;
        }
    }
    else {
        cells_[y][x].amount = 0;
        cells_[y][x].blockType = BlockType::AIR;
    }
}

void Inventory::setSize(const sf::Vector2i& size) {
    size_ = size;
    cells_.resize(size.y);
    for (int y = 0; y < size.y; y++) {
        cells_[y].resize(size.x);
    }
}

const sf::Vector2i &Inventory::getSize() const {
    return size_;
}