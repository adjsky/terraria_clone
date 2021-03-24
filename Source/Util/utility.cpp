//
// Created by adjsky on 12/29/20.
//

#include <cmath>

#include "utility.h"
#include "constants.h"

sf::Vector2i mapGlobalCoordsToGame(float x, float y)
{
    sf::Vector2i gameCoords{ static_cast<int>(std::floor(x / BLOCK_SIZE)), -1 * static_cast<int>(std::floor(y / BLOCK_SIZE)) };
    return gameCoords;
}

sf::Vector2i mapGlobalCoordsToGame(sf::Vector2f globalCoords)
{
    return mapGlobalCoordsToGame(globalCoords.x, globalCoords.y);
}
