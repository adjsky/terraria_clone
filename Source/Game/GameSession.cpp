//
// Created by adjsky on 18.01.2021.
//

#include "GameSession.h"
#include "../ResourceManager/ResourceManager.h"
#include "../InputHandler/InputHandler.h"
#include "../Maths/Math.h"
#include "../Physics/Collisions.h"

GameSession::GameSession(sf::RenderWindow& window, Interface& gui) :
    window_{ window },
    gui_{ gui },
    player_{ },
    view_{sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT) },
    world_{ },
    noclip_{ },
    drawHitBoxes_{ },
    paused_{ false }
{


    player_.move(0.0f, -63.0f * BLOCK_SIZE);
    player_.setOrigin(PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT / 2.0f);
    player_.setScale((float)BLOCK_SIZE / PLAYER_WIDTH * 1.5f, (float)BLOCK_SIZE / PLAYER_HEIGHT * 3);
    player_.setTimeStep(0.2f);
    player_.constructHitBox();

    Animation& moveAnimation = player_.getAnimation(Player::MOVE);
    moveAnimation.setSpriteSheet(ResourceManager::getTexture(ResourceManager::PLAYER));
    moveAnimation.addFrame(sf::IntRect{ 65, 0, PLAYER_WIDTH, PLAYER_HEIGHT });

    moveAnimation.addFrame(sf::IntRect{ 119, 0, PLAYER_WIDTH, PLAYER_HEIGHT });
    moveAnimation.addFrame(sf::IntRect{ 172, 0, PLAYER_WIDTH, PLAYER_HEIGHT });
    moveAnimation.addFrame(sf::IntRect{ 226, 0, PLAYER_WIDTH, PLAYER_HEIGHT });
    moveAnimation.addFrame(sf::IntRect{ 280, 0, PLAYER_WIDTH, PLAYER_HEIGHT });

    Animation& standAnimation = player_.getAnimation(Player::STAND);
    standAnimation.setSpriteSheet(ResourceManager::getTexture(ResourceManager::PLAYER));
    standAnimation.addFrame(sf::IntRect{ 9, 0, PLAYER_WIDTH, PLAYER_HEIGHT });

    Animation& jumpAnimation = player_.getAnimation(Player::JUMP);
    jumpAnimation.setSpriteSheet(ResourceManager::getTexture(ResourceManager::PLAYER));
    jumpAnimation.addFrame(sf::IntRect{ 383, 0, PLAYER_WIDTH, PLAYER_HEIGHT });

    player_.setAnimation(standAnimation);

    gui_.showHotBar();
    gui_.updateHealth(player_);
    gui_.updateHotBar(player_);
    gui_.highlightHotBarCell(player_);
}

void GameSession::update(float delta) {
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Tab) == InputHandler::JUST_PRESSED) {
        gui_.showInventory();
        paused_ = !paused_;
    }
    if (!paused_) {
        if (InputHandler::getMouseButtonState(sf::Mouse::Left) == InputHandler::JUST_PRESSED) {
            window_.setView(view_);
            sf::Vector2f globalCoords{ window_.mapPixelToCoords(sf::Mouse::getPosition(window_)) };
            sf::Vector2i pos{ mapGlobalCoordsToGame(globalCoords) };
            if (math::distanceBetween(mapGlobalCoordsToGame(player_.getPosition()), pos) <= BREAK_PLACE_DISTANCE) {
                const Block* block { world_.destroyBlock(pos) };
                if (block) {
                    player_.getInventory().addItem(block->type, 1);
                    gui_.updateHotBar(player_);
                }
            }
            window_.setView(window_.getDefaultView());
        }
        if (InputHandler::getMouseButtonState(sf::Mouse::Right) == InputHandler::JUST_PRESSED) {
            window_.setView(view_);
            sf::Vector2f globalCoords{ window_.mapPixelToCoords(sf::Mouse::getPosition(window_)) };
            sf::Vector2i pos{ mapGlobalCoordsToGame(globalCoords) };
            if (math::distanceBetween(mapGlobalCoordsToGame(player_.getPosition()), pos) <= BREAK_PLACE_DISTANCE &&
                canPlaceBlock(player_, pos, world_))
            {
                const InventoryCell& cell {player_.getInventory().getCell(player_.getHeldItem(), 0) };
                if (cell.amount != 0) {
                    world_.placeBlock(pos, cell.blockType);
                    player_.getInventory().removeItem(player_.getHeldItem(), 0, 1);
                    gui_.updateHotBar(player_);
                }
            }
            window_.setView(window_.getDefaultView());
        }
        if (InputHandler::getKeyboardKeyState(sf::Keyboard::X) == InputHandler::JUST_PRESSED) {
            noclip_ = !noclip_;
            player_.verticalSpeed = 0;
            player_.setAnimation(player_.getAnimation(Player::STAND));
        }
        if (InputHandler::getKeyboardKeyState(sf::Keyboard::Tilde) == InputHandler::JUST_PRESSED) {
            drawHitBoxes_ = !drawHitBoxes_;
        }
        for (int i = sf::Keyboard::Num1; i <= sf::Keyboard::Num9; i++) {
            if (InputHandler::getKeyboardKeyState(static_cast<sf::Keyboard::Key>(i)) == InputHandler::JUST_PRESSED) {
                player_.setHeldItem(i - sf::Keyboard::Num1);
                gui_.highlightHotBarCell(player_);
            }
        }
        if (InputHandler::getKeyboardKeyState(sf::Keyboard::Num0) == InputHandler::JUST_PRESSED) {
            player_.setHeldItem(9);
            gui_.highlightHotBarCell(player_);
        }
    }
}

void GameSession::fixedUpdate(float fixedDelta) {
    if (!paused_) {
        if (!noclip_) {
            if (player_.getDistanceToGround(world_) < 0.5f) {
                player_.isOnGround = true;
            }
            else {
                player_.isOnGround = false;
            }
            if (!player_.isOnGround) {
                player_.verticalSpeed += GAME_SPEED / 15.0;
            }
        }

        bool moved = false;
        if (InputHandler::getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::JUST_PRESSED ||
            InputHandler::getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::STILL_PRESSED)
        {
            if (player_.isOnGround && !noclip_) {
                player_.verticalSpeed = -GAME_SPEED * 2.0f;
                player_.isOnGround = false;
                player_.setAnimation(player_.getAnimation(Player::JUMP));
                moved = true;
            }
        }
        if (InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::JUST_PRESSED ||
            InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::STILL_PRESSED)
        {
            if (noclip_) {
                player_.move(-GAME_SPEED * 3.0f, 0.0f);
            }
            else {
                player_.horizontalSpeed = -GAME_SPEED;
                if (player_.isOnGround) {
                    player_.setAnimation(player_.getAnimation(Player::MOVE));
                    moved = true;
                }
            }
            player_.changeDirection(Player::MoveDirections::LEFT);
        }
        if (InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::JUST_PRESSED ||
            InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::STILL_PRESSED)
        {
            if (noclip_) {
                player_.move(GAME_SPEED * 3.0f, 0.0f);
            }
            else {
                player_.horizontalSpeed = GAME_SPEED;
                if (player_.isOnGround) {
                    player_.setAnimation(player_.getAnimation(Player::MOVE));
                    moved = true;
                }
            }
            player_.changeDirection(Player::MoveDirections::RIGHT);
        }
        if (InputHandler::getKeyboardKeyState(sf::Keyboard::W) == InputHandler::JUST_PRESSED ||
            InputHandler::getKeyboardKeyState(sf::Keyboard::W) == InputHandler::STILL_PRESSED)
        {
            if (noclip_) {
                player_.move(0.0f, -GAME_SPEED * 2.0f);
            }
        }
        if (InputHandler::getKeyboardKeyState(sf::Keyboard::S) == InputHandler::JUST_PRESSED ||
            InputHandler::getKeyboardKeyState(sf::Keyboard::S) == InputHandler::STILL_PRESSED)
        {
            if (noclip_) {
                player_.move(0.0f, GAME_SPEED * 2.0f);
            }
        }

        if (!moved && player_.isOnGround) {
            player_.setAnimation(player_.getAnimation(Player::STAND));
        }

        if (!noclip_) {
            player_.moveWithCollide(world_);
        }

        player_.update(fixedDelta);

        if (!noclip_) {
            player_.horizontalSpeed = 0;
        }

        view_.setCenter(player_.getPosition());
    }
}

void GameSession::render() {
    window_.setView(view_);
    world_.draw(window_);
    window_.draw(player_);
    if (drawHitBoxes_) {
        window_.draw(player_.getHitBox());
    }
    window_.setView(window_.getDefaultView());
}