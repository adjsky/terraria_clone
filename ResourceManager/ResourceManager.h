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
    static void initialize();
    static std::unique_ptr<sf::Texture>& getBlockTexture();
    static std::unique_ptr<sf::Font>& getFont();
private:
    static std::unique_ptr<sf::Texture> blocksSpriteSheet;
    static std::unique_ptr<sf::Font> font;
};


#endif //TERRARIA_CLONE_RESOURCEMANAGER_H
