#include "Game/Game.h"
#include "ResourceManager/ResourceManager.h"
#include "World/World.h"

int main() {
    ResourceManager::initialize();
    World::initialize();
    Game game;
    game.start();
    return 0;
}
