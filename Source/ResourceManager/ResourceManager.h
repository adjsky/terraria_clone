//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_RESOURCEMANAGER_H
#define TERRARIA_CLONE_RESOURCEMANAGER_H

#include <array>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include "../World/Block/Block.h"

class ResourceManager {
public:
    enum Textures {
        BLOCK,
        PLAYER,
        TEXTURES_COUNT
    };

    static void initialize();
    static const sf::Texture& getTexture(Textures texture);
    static const sf::Font& getFont();

private:
    static std::array<std::unique_ptr<sf::Texture>, TEXTURES_COUNT> textures;
    static std::unique_ptr<sf::Font> font;
};

#endif //TERRARIA_CLONE_RESOURCEMANAGER_H