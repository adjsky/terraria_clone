#include "Game/Game.h"
#include "ResourceManager/ResourceManager.h"

int main() {
    ResourceManager::initialize();
    Game game;
    game.start();
    return 0;
}
