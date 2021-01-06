//
// Created by adjsky on 12/29/20.
//

#ifndef TERRARIA_CLONE_UTILITY_H
#define TERRARIA_CLONE_UTILITY_H

#include <SFML/System/Vector2.hpp>
#include <cmath>

sf::Vector2i mapGlobalCoordsToGame(float x, float y);
sf::Vector2i mapGlobalCoordsToGame(sf::Vector2f globalCoords);

#endif //TERRARIA_CLONE_UTILITY_H
