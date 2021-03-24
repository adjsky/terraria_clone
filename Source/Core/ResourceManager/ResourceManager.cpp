//
// Created by adjsky on 12/28/20.
//

#include <stdexcept>

#include "ResourceManager.h"


ResourceManager::ResourceManager() :
    textures_{},
    fonts_{}
{
    std::generate(textures_.begin(), textures_.end(), []()
    { return std::make_unique<sf::Texture>(); });
    if (!textures_[ITEMS]->loadFromFile("../Resources/Spritesheets/item_sprites.png")) {
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
    if (!textures_[BUTTONS]->loadFromFile("../Resources/Spritesheets/buttons.png")) {
        throw std::runtime_error("Couldn't load buttons textures");
    }
    if (!textures_[GAME_MENU_BACKGROUND]->loadFromFile("../Resources/Textures/black_background.jpg")) {
        throw std::runtime_error("Couldn't load game menu background texture");
    }

    // fill block rects
    std::size_t iBlock{ static_cast<std::size_t>(ItemTypes::BLOCK) };
    std::size_t nBlockTypes{ static_cast<std::size_t>(BlockType::TYPES_COUNT) };
    textureRects_[iBlock].resize(nBlockTypes);
    for (std::size_t i = 0; i < nBlockTypes; i++) {
        textureRects_[iBlock][i].width = 64;
        textureRects_[iBlock][i].height = 64;
        textureRects_[iBlock][i].left = (64 + 1) * i;
    }

    std::size_t iSword{ static_cast<std::size_t>(ItemTypes::SWORD) };
    std::size_t nSwordTypes{ static_cast<std::size_t>(SwordType::TYPES_COUNT) };
    textureRects_[iSword].resize(nSwordTypes);
    for (std::size_t i = 0; i < nSwordTypes; i++) {
        textureRects_[iSword][i].top = 65;
        textureRects_[iSword][i].width = 22;
        textureRects_[iSword][i].height = 22;
        textureRects_[iSword][i].left = 22 * i;
    }
}

void ResourceManager::initializeFonts()
{
    fonts_[ROBOTO] = std::make_unique<tgui::Font>("../Resources/Fonts/Roboto/Roboto-Medium.ttf");
    fonts_[YUSEI] = std::make_unique<tgui::Font>("../Resources/Fonts/YuseiMagic.ttf");
    fonts_[POTTA_ONE] = std::make_unique<tgui::Font>("../Resources/Fonts/PottaOne.ttf");
}

const sf::Texture& ResourceManager::getTexture(Textures texture)
{
    return *textures_[texture];
}

sf::IntRect ResourceManager::getTextureRect(ItemTypes type, int id)
{
    return textureRects_[static_cast<std::size_t>(type)][id];
}

const tgui::Font& ResourceManager::getFont(Fonts font)
{
    return *fonts_[font];
}
