//
// Created by adjsky on 12/28/20.
//

#include <stdexcept>

#include "ResourceManager.h"

std::unique_ptr<sf::Texture> ResourceManager::blocksSpriteSheet{ std::make_unique<sf::Texture>() };
std::unique_ptr<sf::Font> ResourceManager::font{ std::make_unique<sf::Font>() };

void ResourceManager::initialize() {
    if (!blocksSpriteSheet->loadFromFile("../Resources/Spritesheets/block_sprites.png")) {
        throw std::runtime_error("Couldn't load block sprites");
    }
    if (!font->loadFromFile("../Resources/Fonts/Roboto/Roboto-Medium.ttf")) {
        throw std::runtime_error("Couldn't load Roboto-Medium font");
    }
}

std::unique_ptr<sf::Texture> &ResourceManager::getBlockTexture() {
    return blocksSpriteSheet;
}

std::unique_ptr<sf::Font> &ResourceManager::getFont() {
    return font;
}
