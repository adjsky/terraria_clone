//
// Created by adjsky on 03.02.2021.
//

#ifndef TERRARIA_CLONE_GAMELOGIC_H
#define TERRARIA_CLONE_GAMELOGIC_H

#include "../Events/Events.h"

// forward declare game class and link it in cpp file
class Game;

class GameLogic
{
public:
    GameLogic(Game& gameInstance);

    void update(float deltaTime);
    void fixedUpdate(float fixedDelta);

    void hotBarCellPress(const GameEvent::HotBarCellPressed& event);
    void inventoryCellPress(const GameEvent::InventoryCellPressed& event);
    void closeGame(const GameEvent::ExitButtonClicked& event);
    void closeMenu(const GameEvent::ContinueGameButtonClicked& event);
    void continueGame(const GameEvent::ContinueMenuButtonClicked& event);
    void createNewWorld(const GameEvent::NewWorldButtonClicked& event);
    void openMainMenu(const GameEvent::MainMenuButtonClicked& event);

    void destroyBlock();

private:
    Game& gameInstance_;
    bool drawHitBoxes_;
    bool noclip_;
    float breakTimer_;
};

#endif //TERRARIA_CLONE_GAMELOGIC_H
