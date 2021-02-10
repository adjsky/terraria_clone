//
// Created by adjsky on 03.02.2021.
//

#include "GameLogic.h"
#include "../Maths/Math.h"
#include "../Physics/Collisions.h"
#include "../Core/Engine.h"
#include "../Util/Serialization/GameSerialization.h"

GameLogic::GameLogic() :
    drawHitBoxes_{ false },
    noclip_{ false }
{
    Engine::getEventSystem()->sink<GameEvent::HotBarCellPressed>().connect<&GameLogic::hotBarCellPress>(this);
    Engine::getEventSystem()->sink<GameEvent::InventoryCellPressed>().connect<&GameLogic::inventoryCellPress>(this);
    Engine::getEventSystem()->sink<GameEvent::ExitButtonClicked>().connect<&GameLogic::closeGame>(this);
    Engine::getEventSystem()->sink<GameEvent::ContinueGameButtonClicked>().connect<&GameLogic::closeMenu>(this);
    Engine::getEventSystem()->sink<GameEvent::ContinueMenuButtonClicked>().connect<&GameLogic::continueGame>(this);
    Engine::getEventSystem()->sink<GameEvent::NewWorldButtonClicked>().connect<&GameLogic::createNewWorld>(this);
    Engine::getEventSystem()->sink<GameEvent::MainMenuButtonClicked>().connect<&GameLogic::openMainMenu>(this);
}

void GameLogic::update(float deltaTime) {
    auto* inputHandler{ Engine::getInputHandler() };
    auto* gameInstance{ Engine::getGameInstance() };
    auto* gameSession{ gameInstance->getGameSession() };
    auto& window{ gameInstance->getWindow() };
    auto& camera{ gameInstance->getCamera() };
    auto& player{ gameInstance->getGameSession()->getPlayer() };
    auto& gui{ gameInstance->getInterface() };
    if (player.hasAttachedItem) {
        gui.updateAttachedItem(player);
    }
    if (inputHandler->getKeyboardKeyState(sf::Keyboard::Tab) == InputHandler::JUST_PRESSED) {
        gui.showInventory(!gui.inventoryIsOpen());
        gameInstance->pause(!gameInstance->isPaused());
    }
    if (inputHandler->getKeyboardKeyState(sf::Keyboard::Slash) == InputHandler::JUST_PRESSED) {
        gui.showConsole(!gui.consoleIsShown());
        gameInstance->pause(!gameInstance->isPaused());
    }
    if (inputHandler->getKeyboardKeyState(sf::Keyboard::Escape) == InputHandler::JUST_PRESSED) {
        gui.showGameMenu(!gui.gameMenuIsOpen());
        gameInstance->pause(!gameInstance->isPaused());
    }
    if (!gameInstance->isPaused()) {
        if (inputHandler->getMouseButtonState(sf::Mouse::Left) == InputHandler::JUST_PRESSED) {
            window.setView(camera);
            sf::Vector2f globalCoords{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) };
            sf::Vector2i pos{ mapGlobalCoordsToGame(globalCoords) };
            if (math::distanceBetween(mapGlobalCoordsToGame(gameSession->getPlayer().getPosition()), pos) <= BREAK_PLACE_DISTANCE) {
                const Block* block { gameSession->getWorld().destroyBlock(pos.x, pos.y) };
                if (block) {
                    Inventory::Cell cell{};
                    cell.id = block->type;
                    cell.itemType = ItemTypes::BLOCK;
                    cell.amount = 1;
                    if (gameSession->getPlayer().getHotBar().addItem(cell)) {
                        gui.updateHotBar(gameSession->getPlayer());
                    }
                    else {
                        gameSession->getPlayer().getBackpack().addItem(cell);
                        gui.updateInventory(gameSession->getPlayer());
                    }
                }
            }
            window.setView(window.getDefaultView());
        }
        if (inputHandler->getMouseButtonState(sf::Mouse::Right) == InputHandler::JUST_PRESSED) {
            window.setView(camera);
            sf::Vector2f globalCoords{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) };
            sf::Vector2i pos{ mapGlobalCoordsToGame(globalCoords) };
            if (math::distanceBetween(mapGlobalCoordsToGame(gameSession->getPlayer().getPosition()), pos) <= BREAK_PLACE_DISTANCE &&
                canPlaceBlock(gameSession->getPlayer(), pos, gameSession->getWorld()))
            {
                const Inventory::Cell& cell { gameSession->getPlayer().getHotBar().getCell(gameSession->getPlayer().getHotBarIndex(), 0) };
                if (cell.itemType == ItemTypes::BLOCK && cell.amount != 0) {
                    bool placed{ gameSession->getWorld().placeBlock(pos.x, pos.y,
                                                                    static_cast<BlockType::Type>(cell.id)) };
                    if (placed) {
                        gameSession->getPlayer().getHotBar().removeItem(gameSession->getPlayer().getHotBarIndex(), 0, 1);
                        gui.updateHotBar(gameSession->getPlayer());
                    }
                }
            }
            window.setView(window.getDefaultView());
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::X) == InputHandler::JUST_PRESSED) {
            noclip_ = !noclip_;
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::Tilde) == InputHandler::JUST_PRESSED) {
            drawHitBoxes_ = !drawHitBoxes_;
            gameInstance->drawHitBoxes(drawHitBoxes_);
        }
        for (int i = sf::Keyboard::Num1; i <= sf::Keyboard::Num9; i++) {
            if (inputHandler->getKeyboardKeyState(static_cast<sf::Keyboard::Key>(i)) == InputHandler::JUST_PRESSED) {
                player.setHotBarIndex(i - sf::Keyboard::Num1);
                gui.highlightHotBarCell(player);
            }
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::Num0) == InputHandler::JUST_PRESSED) {
            player.setHotBarIndex(9);
            gui.highlightHotBarCell(player);
        }
    }
}

void GameLogic::fixedUpdate(float fixedDelta) {
    auto* inputHandler{ Engine::getInputHandler() };
    auto* gameInstance{ Engine::getGameInstance() };
    auto& world{ gameInstance->getGameSession()->getWorld() };
    auto& player{ gameInstance->getGameSession()->getPlayer() };
    if (!gameInstance->isPaused()) {
        // update physics
        if (!noclip_) {
            if (player.getDistanceToGround(world) < 0.5f) {
                player.isOnGround = true;
            }
            else {
                player.isOnGround = false;
            }
            if (!player.isOnGround) {
                player.verticalSpeed += GAME_SPEED / 15.0;
            }
        }

        bool moved;

        if (inputHandler->getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::JUST_PRESSED ||
            inputHandler->getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::STILL_PRESSED)
        {
            bool moved = false;
            if (player.isOnGround && !noclip_) {
                player.verticalSpeed = -GAME_SPEED * 2.5f;
                player.isOnGround = false;
                player.setAnimation(player.getAnimation(Player::JUMP));
                moved = true;
            }
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::A) == InputHandler::JUST_PRESSED ||
            inputHandler->getKeyboardKeyState(sf::Keyboard::A) == InputHandler::STILL_PRESSED)
        {
            if (noclip_) {
                player.move(-GAME_SPEED * 3.0f, 0.0f);
            }
            else {
                player.horizontalSpeed = -GAME_SPEED;
                if (player.isOnGround) {
                    player.setAnimation(player.getAnimation(Player::MOVE));
                    moved = true;
                }
            }
            player.changeDirection(Player::MoveDirections::LEFT);
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::D) == InputHandler::JUST_PRESSED ||
            inputHandler->getKeyboardKeyState(sf::Keyboard::D) == InputHandler::STILL_PRESSED)
        {
            if (noclip_) {
                player.move(GAME_SPEED * 3.0f, 0.0f);
            }
            else {
                player.horizontalSpeed = GAME_SPEED;
                if (player.isOnGround) {
                    player.setAnimation(player.getAnimation(Player::MOVE));
                    moved = true;
                }
            }
            player.changeDirection(Player::MoveDirections::RIGHT);
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::W) == InputHandler::JUST_PRESSED ||
            inputHandler->getKeyboardKeyState(sf::Keyboard::W) == InputHandler::STILL_PRESSED)
        {
            if (noclip_) {
                player.move(0.0f, -GAME_SPEED * 2.0f);
            }
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::S) == InputHandler::JUST_PRESSED ||
            inputHandler->getKeyboardKeyState(sf::Keyboard::S) == InputHandler::STILL_PRESSED)
        {
            if (noclip_) {
                player.move(0.0f, GAME_SPEED * 2.0f);
            }
        }

        if (!moved && player.isOnGround) {
            player.setAnimation(player.getAnimation(Player::STAND));
        }

        if (!noclip_) {
            player.moveWithCollide(world);
        }

        player.update(fixedDelta);

        if (!noclip_) {
            player.horizontalSpeed = 0;
        }

        gameInstance->getCamera().setCenter(player.getPosition());
    }
}

void GameLogic::hotBarCellPress(const GameEvent::HotBarCellPressed& event) {
    GameSession* gameSession{ Engine::getGameInstance()->getGameSession() };
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
    GameSession* gameSession{ Engine::getGameInstance()->getGameSession() };
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

void GameLogic::closeGame(const GameEvent::ExitButtonClicked& event) {
    Engine::getGameInstance()->close();
}

void GameLogic::closeMenu(const GameEvent::ContinueGameButtonClicked& event) {
    Engine::getGameInstance()->getInterface().showGameMenu(false);
    Engine::getGameInstance()->pause(false);
}

void GameLogic::continueGame(const GameEvent::ContinueMenuButtonClicked& event) {
    GameSession* gameSession{ Engine::getGameInstance()->createGameSession() };
    GameSerialization::SerializedData gameData{ GameSerialization::getGameData() };
    gameSession->setWorld(gameData.world);
    gameSession->setPlayer(gameData.player);
}

void GameLogic::createNewWorld(const GameEvent::NewWorldButtonClicked& event) {
    GameSession* gameSession{ Engine::getGameInstance()->createGameSession() };
    gameSession->getWorld().generate();
    gameSession->getPlayer().move(0.0f, -65.0f * BLOCK_SIZE);
}

void GameLogic::openMainMenu(const GameEvent::MainMenuButtonClicked& event) {
    GameSession* gameSession{ Engine::getGameInstance()->getGameSession() };
    GameSerialization::saveGame(*gameSession);
    Engine::getGameInstance()->deleteGameSession();
    Engine::getGameInstance()->getInterface().showMainMenu(true);
    Engine::getGameInstance()->getInterface().showGameInterface(false);
    Engine::getGameInstance()->getInterface().showGameMenu(false);
}
