//
// Created by adjsky on 03.02.2021.
//

#ifndef TERRARIA_CLONE_GAMELOGIC_H
#define TERRARIA_CLONE_GAMELOGIC_H

#include "../Game/GameSession.h"
#include "../Events/Events.h"

class GameLogic {
public:
    GameLogic();

    void update(float deltaTime);
    void fixedUpdate(float fixedDelta);

    void hotBarCellPress(const GameEvent::HotBarCellPressed& event);
    void inventoryCellPress(const GameEvent::InventoryCellPressed& event);
    void closeGame(const GameEvent::ExitButtonClicked& event);
    void closeMenu(const GameEvent::ContinueGameButtonClicked& event);
    void continueGame(const GameEvent::ContinueMenuButtonClicked& event);
    void createNewWorld(const GameEvent::NewWorldButtonClicked& event);
    void openMainMenu(const GameEvent::MainMenuButtonClicked& event);

private:
    bool drawHitBoxes_;
    bool noclip_;
};

#endif //TERRARIA_CLONE_GAMELOGIC_H