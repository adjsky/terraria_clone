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

    void hotBarCellPress(const GameEvent::HotBarCellPressed& event);
    void inventoryCellPress(const GameEvent::InventoryCellPressed& event);
    void showInventory(const GameEvent::InventoryShown& event);
    void showConsole(const GameEvent::ConsoleShown& event);
    void breakBlock(const GameEvent::BlockBroken& event);
    void placeBlock(const GameEvent::BlockPlaced& event);
    void setNoClip(const GameEvent::NoClipSet& event);
    void drawHitBoxes(const GameEvent::HitBoxesDrawn& event);
    void switchHotBar(const GameEvent::HotBarSwitched& event);
    void openMenu(const GameEvent::MenuOpened& event);
    void closeGame(const GameEvent::ExitButtonClicked& event);
    void closeMenu(const GameEvent::ContinueGameButtonClicked& event);
    void continueGame(const GameEvent::ContinueMenuButtonClicked& event);
    void createNewWorld(const GameEvent::NewWorldButtonClicked& event);
    void openMainMenu(const GameEvent::MainMenuButtonClicked& event);
};

#endif //TERRARIA_CLONE_GAMELOGIC_H