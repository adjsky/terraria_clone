//
// Created by adjsky on 03.02.2021.
//

#include "GameLogic.h"
#include "../Maths/Math.h"
#include "../Physics/Collisions.h"
#include "../Core/Engine.h"
#include "../Util/Serialization/GameSerialization.h"
#include "../Game/Game.h"

GameLogic::GameLogic(Game& gameInstance) :
    gameInstance_{gameInstance},
    drawHitBoxes_{false},
    noclip_{false},
    breakTimer_{}
{
    Engine::getEventSystem()->sink<GameEvent::HotBarCellPressed>().connect<&GameLogic::hotBarCellPress>(this);
    Engine::getEventSystem()->sink<GameEvent::InventoryCellPressed>().connect<&GameLogic::inventoryCellPress>(this);
    Engine::getEventSystem()->sink<GameEvent::ExitButtonClicked>().connect<&GameLogic::closeGame>(this);
    Engine::getEventSystem()->sink<GameEvent::ContinueGameButtonClicked>().connect<&GameLogic::closeMenu>(this);
    Engine::getEventSystem()->sink<GameEvent::ContinueMenuButtonClicked>().connect<&GameLogic::continueGame>(this);
    Engine::getEventSystem()->sink<GameEvent::NewWorldButtonClicked>().connect<&GameLogic::createNewWorld>(this);
    Engine::getEventSystem()->sink<GameEvent::MainMenuButtonClicked>().connect<&GameLogic::openMainMenu>(this);
}

void GameLogic::update(float deltaTime)
{
    auto* inputHandler{Engine::getInputHandler()};
    auto* gameSession{gameInstance_.getGameSession()};
    auto* window{Engine::getWindow()};
    auto* gui{Engine::getInterface()};
    auto& camera{gameInstance_.getCamera()};
    auto& player{gameInstance_.getGameSession()->getPlayer()};
    if (player.hasAttachedItem) {
        gui->updateAttachedItem(player);
    }
    if (inputHandler->getKeyboardKeyState(sf::Keyboard::Tab) == InputHandler::JUST_PRESSED) {
        gui->showInventory(!gui->inventoryIsOpen());
        gameInstance_.pause(!gameInstance_.isPaused());
    }
    if (inputHandler->getKeyboardKeyState(sf::Keyboard::Slash) == InputHandler::JUST_PRESSED) {
        gui->showConsole(!gui->consoleIsShown());
        gameInstance_.pause(!gameInstance_.isPaused());
    }
    if (inputHandler->getKeyboardKeyState(sf::Keyboard::Escape) == InputHandler::JUST_PRESSED) {
        gui->showGameMenu(!gui->gameMenuIsOpen());
        gameInstance_.pause(!gameInstance_.isPaused());
    }
    if (!gameInstance_.isPaused()) {
        if (inputHandler->getMouseButtonState(sf::Mouse::Left) == InputHandler::JUST_PRESSED) {
            destroyBlock();
        }
        if (inputHandler->getMouseButtonState(sf::Mouse::Left) == InputHandler::STILL_PRESSED) {
            breakTimer_ += deltaTime;
            if (breakTimer_ > breakTime) {
                breakTimer_ -= breakTime;
                destroyBlock();
            }
        }
        if (inputHandler->getMouseButtonState(sf::Mouse::Right) == InputHandler::JUST_PRESSED) {
            window->setView(camera);
            sf::Vector2f globalCoords{window->mapPixelToCoords(sf::Mouse::getPosition(*window))};
            sf::Vector2i pos{mapGlobalCoordsToGame(globalCoords)};
            if (math::distanceBetween(mapGlobalCoordsToGame(player.getPosition()), pos) <= BREAK_PLACE_DISTANCE &&
                Physics::canPlaceBlock(player, pos, gameSession->getWorld())) {
                const Inventory::Cell& cell{player.getHotBar().getCell(player.getHotBarIndex(), 0)};
                if (cell.itemType == ItemTypes::BLOCK && cell.amount != 0) {
                    bool placed{gameSession->getWorld().placeBlock(pos.x, pos.y,
                                                                   static_cast<BlockType>(cell.id))};
                    if (placed) {
                        player.getHotBar().removeItem(player.getHotBarIndex(), 0, 1);
                        gui->updateHotBar(gameSession->getPlayer());
                    }
                }
            }
            window->setView(window->getDefaultView());
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::X) == InputHandler::JUST_PRESSED) {
            noclip_ = !noclip_;
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::Tilde) == InputHandler::JUST_PRESSED) {
            drawHitBoxes_ = !drawHitBoxes_;
            gameInstance_.drawHitBoxes(drawHitBoxes_);
        }
        for (int i = sf::Keyboard::Num1; i <= sf::Keyboard::Num9; i++) {
            if (inputHandler->getKeyboardKeyState(static_cast<sf::Keyboard::Key>(i)) == InputHandler::JUST_PRESSED) {
                player.setHotBarIndex(i - sf::Keyboard::Num1);
                gui->highlightHotBarCell(player);
            }
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::Num0) == InputHandler::JUST_PRESSED) {
            player.setHotBarIndex(9);
            gui->highlightHotBarCell(player);
        }
    }
}

void GameLogic::fixedUpdate(float fixedDelta)
{
    auto* inputHandler{Engine::getInputHandler()};
    auto& world{gameInstance_.getGameSession()->getWorld()};
    auto& player{gameInstance_.getGameSession()->getPlayer()};
    if (!gameInstance_.isPaused()) {
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

        bool moved{false};

        if (inputHandler->getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::JUST_PRESSED ||
            inputHandler->getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::STILL_PRESSED) {
            if (player.isOnGround && !noclip_) {
                player.verticalSpeed = -GAME_SPEED * 2.5f;
                player.isOnGround = false;
                player.setAnimation(player.getAnimation(Player::JUMP));
            }
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::A) == InputHandler::JUST_PRESSED ||
            inputHandler->getKeyboardKeyState(sf::Keyboard::A) == InputHandler::STILL_PRESSED) {
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
            inputHandler->getKeyboardKeyState(sf::Keyboard::D) == InputHandler::STILL_PRESSED) {
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
            inputHandler->getKeyboardKeyState(sf::Keyboard::W) == InputHandler::STILL_PRESSED) {
            if (noclip_) {
                player.move(0.0f, -GAME_SPEED * 2.0f);
            }
        }
        if (inputHandler->getKeyboardKeyState(sf::Keyboard::S) == InputHandler::JUST_PRESSED ||
            inputHandler->getKeyboardKeyState(sf::Keyboard::S) == InputHandler::STILL_PRESSED) {
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

        gameInstance_.getCamera().setCenter(player.getPosition());
    }
}

void GameLogic::hotBarCellPress(const GameEvent::HotBarCellPressed& event)
{
    auto* gameSession{gameInstance_.getGameSession()};
    auto* gui{Engine::getInterface()};
    if (gui->inventoryIsOpen()) {
        Inventory::Cell cell{gameSession->getPlayer().getHotBar().getCell(event.x, 0)};
        if (gameSession->getPlayer().hasAttachedItem) {
            bool swapped{gameSession->getPlayer().getHotBar().setItem(gameSession->getPlayer().attachedItem, event.x, 0)};
            if (swapped) {
                gameSession->getPlayer().attachedItem = cell;
                gui->updateAttachedItem(gameSession->getPlayer(), true);
            }
            else {
                gameSession->getPlayer().hasAttachedItem = false;
                gui->updateAttachedItem(gameSession->getPlayer());
            }
            gui->updateHotBar(gameSession->getPlayer());
        }
        else {
            if (cell.amount) {
                gameSession->getPlayer().attachedItem = cell;
                gameSession->getPlayer().hasAttachedItem = true;
                gameSession->getPlayer().getHotBar().removeItem(event.x, 0);
                gui->updateHotBar(gameSession->getPlayer());
                gui->updateAttachedItem(gameSession->getPlayer());
            }
        }
    }
}

void GameLogic::inventoryCellPress(const GameEvent::InventoryCellPressed& event)
{
    auto* gameSession{gameInstance_.getGameSession()};
    auto& player{gameSession->getPlayer()};
    auto* gui{Engine::getInterface()};
    if (gui->inventoryIsOpen()) {
        Inventory::Cell cell{gameSession->getPlayer().getBackpack().getCell(event.x, event.y)};
        if (gameSession->getPlayer().hasAttachedItem) {
            bool swapped{player.getBackpack().setItem(gameSession->getPlayer().attachedItem, event.x, event.y)};
            if (swapped) {
                gameSession->getPlayer().attachedItem = cell;
                gui->updateAttachedItem(gameSession->getPlayer(), true);
            }
            else {
                gameSession->getPlayer().hasAttachedItem = false;
                gui->updateAttachedItem(gameSession->getPlayer());
            }
            gui->updateInventory(gameSession->getPlayer());
        }
        else {
            if (cell.amount) {
                gameSession->getPlayer().attachedItem = cell;
                gameSession->getPlayer().hasAttachedItem = true;
                gameSession->getPlayer().getBackpack().removeItem(event.x, event.y);
                gui->updateInventory(gameSession->getPlayer());
                gui->updateAttachedItem(gameSession->getPlayer());
            }
        }
    }
}

void GameLogic::closeGame(const GameEvent::ExitButtonClicked& event)
{
    gameInstance_.close();
}

void GameLogic::closeMenu(const GameEvent::ContinueGameButtonClicked& event)
{
    Engine::getInterface()->showGameMenu(false);
    gameInstance_.pause(false);
}

void GameLogic::continueGame(const GameEvent::ContinueMenuButtonClicked& event)
{
    GameSession* gameSession{gameInstance_.createGameSession()};
    GameSerialization::SerializedData gameData{GameSerialization::getGameData()};
    gameSession->setWorld(gameData.world);
    gameSession->setPlayer(gameData.player);
}

void GameLogic::createNewWorld(const GameEvent::NewWorldButtonClicked& event)
{
    GameSession* gameSession{gameInstance_.createGameSession()};
    gameSession->getWorld().generate();
    gameSession->getPlayer().move(0.0f, -65.0f * BLOCK_SIZE);
}

void GameLogic::openMainMenu(const GameEvent::MainMenuButtonClicked& event)
{
    GameSession* gameSession{gameInstance_.getGameSession()};
    GameSerialization::saveGame(*gameSession);
    gameInstance_.deleteGameSession();
    Engine::getInterface()->showMainMenu(true);
    Engine::getInterface()->showGameInterface(false);
    Engine::getInterface()->showGameMenu(false);
}

void GameLogic::destroyBlock()
{
    auto* gameSession{gameInstance_.getGameSession()};
    auto* window{Engine::getWindow()};
    auto& camera{gameInstance_.getCamera()};
    auto& player{gameSession->getPlayer()};
    auto* gui{Engine::getInterface()};
    window->setView(camera);
    sf::Vector2f globalCoords{window->mapPixelToCoords(sf::Mouse::getPosition(*window))};
    sf::Vector2i pos{mapGlobalCoordsToGame(globalCoords)};
    if (math::distanceBetween(mapGlobalCoordsToGame(player.getPosition()), pos) <= BREAK_PLACE_DISTANCE) {
        const Block* block{gameSession->getWorld().destroyBlock(pos.x, pos.y)};
        if (block) {
            Inventory::Cell cell{};
            cell.id = static_cast<int>(block->id);
            cell.itemType = ItemTypes::BLOCK;
            cell.amount = 1;
            if (player.getHotBar().addItem(cell)) {
                gui->updateHotBar(gameSession->getPlayer());
            }
            else {
                player.getBackpack().addItem(cell);
                gui->updateInventory(gameSession->getPlayer());
            }
        }
    }
    window->setView(window->getDefaultView());
}
