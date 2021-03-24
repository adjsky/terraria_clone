#include "Core/Engine.h"
#include "Game/Game.h"

int main()
{
    Engine::init();
    Game game{ };
    game.start();
    return 0;
}
