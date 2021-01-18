//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_RESOURCEMANAGER_H
#define TERRARIA_CLONE_RESOURCEMANAGER_H

#include <array>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <TGUI/Font.hpp>

#include "../World/Block/Block.h"

class ResourceManager {
public:
    enum Textures {
        BLOCK,
        PLAYER,
        HOTBAR,
        HEALTH,
        INVENTORY,
        INVENTORY_CELL,
        INVENTORY_MARK,
        TEXTURES_COUNT
    };

    enum Fonts {
        ROBOTO,
        YUSEI,
        POTTA_ONE,
        FONTS_COUNT
    };

    static void initializeTextures();
    static void initializeFonts();
    static const sf::Texture& getTexture(Textures texture);
    static const tgui::Font& getFont(Fonts font);

private:
    static std::array<std::unique_ptr<sf::Texture>, TEXTURES_COUNT> textures_;
    static std::array<std::unique_ptr<tgui::Font>, FONTS_COUNT> fonts_;
};

#endif //TERRARIA_CLONE_RESOURCEMANAGER_H