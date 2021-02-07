//
// Created by adjsky on 03.02.2021.
//

#include "GameLogic.h"
#include "../Maths/Math.h"
#include "../Physics/Collisions.h"

GameLogic::GameLogic() :
    gameSession_{ nullptr }
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
}

void GameLogic::setGameSession(GameSession* gameSession) {
    gameSession_ = gameSession;
}

void GameLogic::hotBarCellPress(const GameEvent::HotBarCellPressed& event) {
    if (gameSession_->getInterface().inventoryIsOpen()) {
        Inventory::Cell cell{gameSession_->getPlayer().getHotBar().getCell(event.x, 0) };
        if (gameSession_->getPlayer().hasAttachedItem) {
            bool swapped {gameSession_->getPlayer().getHotBar().setItem(gameSession_->getPlayer().attachedItem, event.x, 0) };
            if (swapped) {
                gameSession_->getPlayer().attachedItem = cell;
                gameSession_->getInterface().updateAttachedItem(gameSession_->getPlayer(), true);
            }
            else {
                gameSession_->getPlayer().hasAttachedItem = false;
                gameSession_->getInterface().updateAttachedItem(gameSession_->getPlayer());
            }
            gameSession_->getInterface().updateHotBar(gameSession_->getPlayer());
        }
        else {
            if (cell.amount) {
                gameSession_->getPlayer().attachedItem = cell;
                gameSession_->getPlayer().hasAttachedItem = true;
                gameSession_->getPlayer().getHotBar().removeItem(event.x, 0);
                gameSession_->getInterface().updateHotBar(gameSession_->getPlayer());
                gameSession_->getInterface().updateAttachedItem(gameSession_->getPlayer());
            }
        }
    }
}

void GameLogic::inventoryCellPress(const GameEvent::InventoryCellPressed& event) {
    if (gameSession_->getInterface().inventoryIsOpen()) {
        Inventory::Cell cell{gameSession_->getPlayer().getBackpack().getCell(event.x, event.y) };
        if (gameSession_->getPlayer().hasAttachedItem) {
            bool swapped{gameSession_->getPlayer().getBackpack().setItem(gameSession_->getPlayer().attachedItem, event.x, event.y) };
            if (swapped) {
                gameSession_->getPlayer().attachedItem = cell;
                gameSession_->getInterface().updateAttachedItem(gameSession_->getPlayer(), true);
            }
            else {
                gameSession_->getPlayer().hasAttachedItem = false;
                gameSession_->getInterface().updateAttachedItem(gameSession_->getPlayer());
            }
            gameSession_->getInterface().updateInventory(gameSession_->getPlayer());
        }
        else {
            if (cell.amount) {
                gameSession_->getPlayer().attachedItem = cell;
                gameSession_->getPlayer().hasAttachedItem = true;
                gameSession_->getPlayer().getBackpack().removeItem(event.x, event.y);
                gameSession_->getInterface().updateInventory(gameSession_->getPlayer());
                gameSession_->getInterface().updateAttachedItem(gameSession_->getPlayer());
            }
        }
    }
}

void GameLogic::showInventory(const GameEvent::InventoryShown& event) {
    gameSession_->getInterface().showInventory(!gameSession_->getInterface().inventoryIsOpen());
    gameSession_->pause(!gameSession_->isPaused());
}

void GameLogic::showConsole(const GameEvent::ConsoleShown& event) {
    gameSession_->getInterface().showConsole(!gameSession_->getInterface().consoleIsShown());
    gameSession_->pause(!gameSession_->isPaused());
}

void GameLogic::breakBlock(const GameEvent::BlockBroken& event) {
    if (math::distanceBetween(mapGlobalCoordsToGame(gameSession_->getPlayer().getPosition()), event.blockPosition) <= BREAK_PLACE_DISTANCE) {
        const Block* block { gameSession_->getWorld().destroyBlock(event.blockPosition.x, event.blockPosition.y) };
        if (block) {
            if (gameSession_->getPlayer().getHotBar().addItem(block->type, 1)) {
                gameSession_->getInterface().updateHotBar(gameSession_->getPlayer());
            }
            else {
                gameSession_->getPlayer().getBackpack().addItem(block->type, 1);
                gameSession_->getInterface().updateInventory(gameSession_->getPlayer());
            }

        }
    }
}

void GameLogic::placeBlock(const GameEvent::BlockPlaced& event) {
    if (math::distanceBetween(mapGlobalCoordsToGame(gameSession_->getPlayer().getPosition()), event.blockPosition) <= BREAK_PLACE_DISTANCE &&
        canPlaceBlock(gameSession_->getPlayer(), event.blockPosition, gameSession_->getWorld()))
    {
        const Inventory::Cell& cell { gameSession_->getPlayer().getHotBar().getCell(gameSession_->getPlayer().getHotBarIndex(), 0) };
        if (cell.amount != 0) {
            bool placed{ gameSession_->getWorld().placeBlock(event.blockPosition.x, event.blockPosition.y, cell.blockType) };
            if (placed) {
                gameSession_->getPlayer().getHotBar().removeItem(gameSession_->getPlayer().getHotBarIndex(), 0, 1);
                gameSession_->getInterface().updateHotBar(gameSession_->getPlayer());
            }
        }
    }
}

void GameLogic::setNoClip(const GameEvent::NoClipSet& event) {
    gameSession_->setNoClip(!gameSession_->isNoClipEnabled());
}

void GameLogic::drawHitBoxes(const GameEvent::HitBoxesDrawn& event) {
    gameSession_->shouldDrawHitBoxes(!gameSession_->hitBoxesAreDrawn());
}

void GameLogic::switchHotBar(const GameEvent::HotBarSwitched& event) {
    gameSession_->getPlayer().setHotBarIndex(event.x);
    gameSession_->getInterface().highlightHotBarCell(gameSession_->getPlayer());
}
