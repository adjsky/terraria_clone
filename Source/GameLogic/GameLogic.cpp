//
// Created by adjsky on 03.02.2021.
//

#include "GameLogic.h"
#include "../Maths/Math.h"
#include "../Physics/Collisions.h"
#include "../Core/Engine.h"
#include "../Util/Serialization/GameSerialization.h"

GameLogic::GameLogic()
{
    Engine::getEventSystem()->sink<GameEvent::HotBarCellPressed>().connect<&GameLogic::hotBarCellPress>(this);
    Engine::getEventSystem()->sink<GameEvent::InventoryCellPressed>().connect<&GameLogic::inventoryCellPress>(this);
    Engine::getEventSystem()->sink<GameEvent::InventoryShown>().connect<&GameLogic::showInventory>(this);
    Engine::getEventSystem()->sink<GameEvent::ConsoleShown>().connect<&GameLogic::showConsole>(this);
    Engine::getEventSystem()->sink<GameEvent::BlockBroken>().connect<&GameLogic::breakBlock>(this);
    Engine::getEventSystem()->sink<GameEvent::BlockPlaced>().connect<&GameLogic::placeBlock>(this);
    Engine::getEventSystem()->sink<GameEvent::NoClipSet>().connect<&GameLogic::setNoClip>(this);
    Engine::getEventSystem()->sink<GameEvent::HitBoxesDrawn>().connect<&GameLogic::drawHitBoxes>(this);
    Engine::getEventSystem()->sink<GameEvent::HotBarSwitched>().connect<&GameLogic::switchHotBar>(this);
    Engine::getEventSystem()->sink<GameEvent::MenuOpened>().connect<&GameLogic::openMenu>(this);
    Engine::getEventSystem()->sink<GameEvent::ExitButtonClicked>().connect<&GameLogic::closeGame>(this);
    Engine::getEventSystem()->sink<GameEvent::ContinueGameButtonClicked>().connect<&GameLogic::closeMenu>(this);
    Engine::getEventSystem()->sink<GameEvent::ContinueMenuButtonClicked>().connect<&GameLogic::continueGame>(this);
    Engine::getEventSystem()->sink<GameEvent::NewWorldButtonClicked>().connect<&GameLogic::createNewWorld>(this);
    Engine::getEventSystem()->sink<GameEvent::MainMenuButtonClicked>().connect<&GameLogic::openMainMenu>(this);
}

void GameLogic::hotBarCellPress(const GameEvent::HotBarCellPressed& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    if ( Engine::getGameInstance()->getInterface().inventoryIsOpen()) {
        Inventory::Cell cell{gameSession->getPlayer().getHotBar().getCell(event.x, 0) };
        if (gameSession->getPlayer().hasAttachedItem) {
            bool swapped { gameSession->getPlayer().getHotBar().setItem(gameSession->getPlayer().attachedItem, event.x, 0) };
            if (swapped) {
                gameSession->getPlayer().attachedItem = cell;
                Engine::getGameInstance()->getInterface().updateAttachedItem(gameSession->getPlayer(), true);
            }
            else {
                gameSession->getPlayer().hasAttachedItem = false;
                Engine::getGameInstance()->getInterface().updateAttachedItem(gameSession->getPlayer());
            }
            Engine::getGameInstance()->getInterface().updateHotBar(gameSession->getPlayer());
        }
        else {
            if (cell.amount) {
                gameSession->getPlayer().attachedItem = cell;
                gameSession->getPlayer().hasAttachedItem = true;
                gameSession->getPlayer().getHotBar().removeItem(event.x, 0);
                Engine::getGameInstance()->getInterface().updateHotBar(gameSession->getPlayer());
                Engine::getGameInstance()->getInterface().updateAttachedItem(gameSession->getPlayer());
            }
        }
    }
}

void GameLogic::inventoryCellPress(const GameEvent::InventoryCellPressed& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    if (Engine::getGameInstance()->getInterface().inventoryIsOpen()) {
        Inventory::Cell cell{gameSession->getPlayer().getBackpack().getCell(event.x, event.y) };
        if (gameSession->getPlayer().hasAttachedItem) {
            bool swapped{gameSession->getPlayer().getBackpack().setItem(gameSession->getPlayer().attachedItem, event.x, event.y) };
            if (swapped) {
                gameSession->getPlayer().attachedItem = cell;
                Engine::getGameInstance()->getInterface().updateAttachedItem(gameSession->getPlayer(), true);
            }
            else {
                gameSession->getPlayer().hasAttachedItem = false;
                Engine::getGameInstance()->getInterface().updateAttachedItem(gameSession->getPlayer());
            }
            Engine::getGameInstance()->getInterface().updateInventory(gameSession->getPlayer());
        }
        else {
            if (cell.amount) {
                gameSession->getPlayer().attachedItem = cell;
                gameSession->getPlayer().hasAttachedItem = true;
                gameSession->getPlayer().getBackpack().removeItem(event.x, event.y);
                Engine::getGameInstance()->getInterface().updateInventory(gameSession->getPlayer());
                Engine::getGameInstance()->getInterface().updateAttachedItem(gameSession->getPlayer());
            }
        }
    }
}

void GameLogic::showInventory(const GameEvent::InventoryShown& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    Engine::getGameInstance()->getInterface().showInventory(! Engine::getGameInstance()->getInterface().inventoryIsOpen());
    gameSession->pause(!gameSession->isPaused());
}

void GameLogic::showConsole(const GameEvent::ConsoleShown& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    Engine::getGameInstance()->getInterface().showConsole(! Engine::getGameInstance()->getInterface().consoleIsShown());
    gameSession->pause(!gameSession->isPaused());
}

void GameLogic::breakBlock(const GameEvent::BlockBroken& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    if (math::distanceBetween(mapGlobalCoordsToGame(gameSession->getPlayer().getPosition()), event.blockPosition) <= BREAK_PLACE_DISTANCE) {
        const Block* block { gameSession->getWorld().destroyBlock(event.blockPosition.x, event.blockPosition.y) };
        if (block) {
            if (gameSession->getPlayer().getHotBar().addItem(block->type, 1)) {
                Engine::getGameInstance()->getInterface().updateHotBar(gameSession->getPlayer());
            }
            else {
                gameSession->getPlayer().getBackpack().addItem(block->type, 1);
                Engine::getGameInstance()->getInterface().updateInventory(gameSession->getPlayer());
            }

        }
    }
}

void GameLogic::placeBlock(const GameEvent::BlockPlaced& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    if (math::distanceBetween(mapGlobalCoordsToGame(gameSession->getPlayer().getPosition()), event.blockPosition) <= BREAK_PLACE_DISTANCE &&
        canPlaceBlock(gameSession->getPlayer(), event.blockPosition, gameSession->getWorld()))
    {
        const Inventory::Cell& cell { gameSession->getPlayer().getHotBar().getCell(gameSession->getPlayer().getHotBarIndex(), 0) };
        if (cell.amount != 0) {
            bool placed{ gameSession->getWorld().placeBlock(event.blockPosition.x, event.blockPosition.y, cell.blockType) };
            if (placed) {
                gameSession->getPlayer().getHotBar().removeItem(gameSession->getPlayer().getHotBarIndex(), 0, 1);
                Engine::getGameInstance()->getInterface().updateHotBar(gameSession->getPlayer());
            }
        }
    }
}

void GameLogic::setNoClip(const GameEvent::NoClipSet& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    gameSession->setNoClip(!gameSession->isNoClipEnabled());
}

void GameLogic::drawHitBoxes(const GameEvent::HitBoxesDrawn& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    gameSession->shouldDrawHitBoxes(!gameSession->hitBoxesAreDrawn());
}

void GameLogic::switchHotBar(const GameEvent::HotBarSwitched& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    gameSession->getPlayer().setHotBarIndex(event.x);
    Engine::getGameInstance()->getInterface().highlightHotBarCell(gameSession->getPlayer());
}

void GameLogic::openMenu(const GameEvent::MenuOpened& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    gameSession->pause(!gameSession->isPaused());
    Engine::getGameInstance()->getInterface().showGameMenu(!Engine::getGameInstance()->getInterface().gameMenuIsOpen());
}

void GameLogic::closeGame(const GameEvent::ExitButtonClicked& event) {
    Engine::getGameInstance()->close();
}

void GameLogic::closeMenu(const GameEvent::ContinueGameButtonClicked& event) {
    Engine::getGameInstance()->getInterface().showGameMenu(false);
    Engine::getGameInstance()->getGameSession()->pause(false);
}

void GameLogic::continueGame(const GameEvent::ContinueMenuButtonClicked& event) {
    auto gameSession{ Engine::getGameInstance()->createGameSession() };
    GameSerialization::SerializedData gameData{ GameSerialization::getGameData() };
    gameSession->setWorld(gameData.world);
    gameSession->setPlayer(gameData.player);
    gameSession->initializePlayer();
    gameSession->initializeAnimations();
    gameSession->initializeInterface();
}

void GameLogic::createNewWorld(const GameEvent::NewWorldButtonClicked& event) {
    auto gameSession{ Engine::getGameInstance()->createGameSession() };
    gameSession->getWorld().generate();
    gameSession->getPlayer().move(0.0f, -65.0f * BLOCK_SIZE);
    gameSession->initializePlayer();
    gameSession->initializeAnimations();
    gameSession->initializeInterface();
}

void GameLogic::openMainMenu(const GameEvent::MainMenuButtonClicked& event) {
    auto gameSession{ Engine::getGameInstance()->getGameSession() };
    GameSerialization::saveGame(*gameSession);
    Engine::getGameInstance()->deleteGameSession();
    Engine::getGameInstance()->getInterface().showMainMenu(true);
    Engine::getGameInstance()->getInterface().showGameInterface(false);
    Engine::getGameInstance()->getInterface().showGameMenu(false);
}