//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_RESOURCEMANAGER_H
#define TERRARIA_CLONE_RESOURCEMANAGER_H

#include <array>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <TGUI/Font.hpp>

#include "../../World/Block/Block.h"

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

    ResourceManager();

    const sf::Texture& getTexture(Textures texture);
    const tgui::Font& getFont(Fonts font);

    // can't initialize tgui fonts before Interface class is constructed so i need to call it manually after creating this class
    void initializeFonts();

private:
    std::array<std::unique_ptr<sf::Texture>, TEXTURES_COUNT> textures_;
    std::array<std::unique_ptr<tgui::Font>, FONTS_COUNT> fonts_;
};

#endif //TERRARIA_CLONE_RESOURCEMANAGER_H