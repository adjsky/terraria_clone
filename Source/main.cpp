#include "ResourceManager/ResourceManager.h"
#include "World/World.h"
#include "Game/Game.h"

int main() {
    ResourceManager::initialize();
    World::initialize();
    Game game{};
    game.start();
    return 0;
}