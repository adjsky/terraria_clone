//
// Created by adjsky on 12/30/20.
//

#include "../World/World.h"
#include "../Entities/Player/Player.h"
#include "../Util/utility.h"
#include "../Maths/Math.h"

#ifndef TERRARIA_CLONE_COLLISIONS_H
#define TERRARIA_CLONE_COLLISIONS_H

namespace Physics
{
bool canPlaceBlock(const Player& player, const sf::Vector2i& pos, const World& world);
}

#endif //TERRARIA_CLONE_COLLISIONS_H