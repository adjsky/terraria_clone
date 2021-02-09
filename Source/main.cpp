#include "Core/Engine.h"

int main() {
    Engine::init();
    Engine::getGameInstance()->start();
    return 0;
}