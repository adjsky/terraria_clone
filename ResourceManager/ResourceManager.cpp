//
// Created by adjsky on 12/28/20.
//

#include <stdexcept>

#include "ResourceManager.h"

std::array<std::unique_ptr<sf::Texture>, ResourceManager::TEXTURES_COUNT> ResourceManager::textures{ };
std::unique_ptr<sf::Font> ResourceManager::font{ std::make_unique<sf::Font>() };

void ResourceManager::initialize() {
    std::generate(textures.begin(), textures.end(), []() { return std::make_unique<sf::Texture>(); });
    if (!textures[BLOCK]->loadFromFile("../Resources/Spritesheets/block_sprites.png")) {
        throw std::runtime_error("Couldn't load block sprites");
    }
    if (!textures[PLAYER]->loadFromFile("../Resources/Spritesheets/player_sprites.png")) {
        throw std::runtime_error("Couldn't load player sprites");
    }
    if (!font->loadFromFile("../Resources/Fonts/Roboto/Roboto-Medium.ttf")) {
        throw std::runtime_error("Couldn't load Roboto-Medium font");
    }
}

const sf::Texture& ResourceManager::getTexture(Textures texture) {
    return *textures[texture];
}

const sf::Font& ResourceManager::getFont() {
    return *font;
}
