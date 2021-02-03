#include "Core/Engine.h"
#include "Game/Game.h"

int main() {
    Engine::init();
    sf::ContextSettings context{};
    Game game{ context };
    game.start();
    return 0;
}