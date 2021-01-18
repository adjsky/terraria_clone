#include "ResourceManager/ResourceManager.h"
#include "Game/Game.h"

int main() {
    ResourceManager::initializeTextures();
    sf::ContextSettings context{};
    Game game{ context };
    game.start();
    return 0;
}