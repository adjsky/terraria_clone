//
// Created by adjsky on 12/29/20.
//

#include "utility.h"
#include "constants.h"

sf::Vector2i mapGlobalCoordsToGame(float x, float y) {
    sf::Vector2i gameCoords{ (int)std::floor(x / BLOCK_SIZE), -1 * (int)std::floor(y / BLOCK_SIZE) };
    return gameCoords;
}

sf::Vector2i mapGlobalCoordsToGame(const sf::Vector2f& globalCoords) {
    return mapGlobalCoordsToGame(globalCoords.x, globalCoords.y);
}