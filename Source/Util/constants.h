//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_CONSTANTS_H
#define TERRARIA_CLONE_CONSTANTS_H

#include <SFML/System/Vector2.hpp>

constexpr int HEIGHT = 720;

constexpr int WIDTH = 1280;

constexpr int VIEW_HEIGHT = 720;

constexpr int VIEW_WIDTH = 1280;

constexpr int BLOCK_SIZE = 32;

constexpr int CHUNK_WIDTH = 16;

constexpr int CHUNK_HEIGHT = 255;

constexpr int WORLD_HEIGHT_GENERATION = 60;

constexpr int BREAK_PLACE_DISTANCE = 10;

constexpr int GAME_SPEED = BLOCK_SIZE / 16;

constexpr int PLAYER_WIDTH = 45;

constexpr int PLAYER_HEIGHT = 90;

const sf::Vector2i PLAYER_BACKPACK_SIZE{ 9, 10 };

constexpr int PLAYER_HOTBAR_SIZE = 10;

constexpr float breakTime = 0.5f;

#endif //TERRARIA_CLONE_CONSTANTS_H
