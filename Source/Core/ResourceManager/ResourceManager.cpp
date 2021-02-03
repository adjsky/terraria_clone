//
// Created by adjsky on 12/28/20.
//

#include <stdexcept>

#include "ResourceManager.h"

ResourceManager::ResourceManager() :
    textures_{ },
    fonts_{ }
{
    std::generate(textures_.begin(), textures_.end(), [](){ return std::make_unique<sf::Texture>(); });
    if (!textures_[BLOCK]->loadFromFile("../Resources/Spritesheets/block_sprites.png")) {
        throw std::runtime_error("Couldn't load block sprites");
    }
    if (!textures_[PLAYER]->loadFromFile("../Resources/Spritesheets/player_sprites.png")) {
        throw std::runtime_error("Couldn't load player sprites");
    }
    if (!textures_[HEALTH]->loadFromFile("../Resources/Spritesheets/hearts.png")) {
        throw std::runtime_error("Couldn't load hearts sprite");
    }
    if (!textures_[INVENTORY_CELL]->loadFromFile("../Resources/Spritesheets/inventory_cells.png")) {
        throw std::runtime_error("Couldn't load inventory cells sprite");
    }
    if (!textures_[HOTBAR]->loadFromFile("../Resources/Textures/hotbar.png")) {
        throw std::runtime_error("Couldn't load hotbar texture");
    }
    if (!textures_[INVENTORY]->loadFromFile("../Resources/Textures/inventory.png")) {
        throw std::runtime_error("Couldn't load inventory texture");
    }
    if (!textures_[INVENTORY_MARK]->loadFromFile("../Resources/Textures/inventory_mark.png")) {
        throw std::runtime_error("Couldn't load inventory mark texture");
    }
}

void ResourceManager::initializeFonts() {
    fonts_[ROBOTO] = std::make_unique<tgui::Font>("../Resources/Fonts/Roboto/Roboto-Medium.ttf");
    fonts_[YUSEI] = std::make_unique<tgui::Font>("../Resources/Fonts/YuseiMagic.ttf");
    fonts_[POTTA_ONE] = std::make_unique<tgui::Font>("../Resources/Fonts/PottaOne.ttf");
}

const sf::Texture& ResourceManager::getTexture(Textures texture) {
    return *textures_[texture];
}

const tgui::Font& ResourceManager::getFont(Fonts font) {
    return *fonts_[font];
}