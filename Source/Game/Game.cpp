//
// Created by adjsky on 12/28/20.
//

#include <iostream>

#include "Game.h"
#include "../InputHandler/InputHandler.h"
#include "../ResourceManager/ResourceManager.h"
#include "../World/World.h"
#include "../Physics/Collisions.h"
#include "../World/Block/BlockTypes.h"

// @TODO
// 1) Система интерфейса
// 2) Написать систему инвентаря
// 3) Фабрика информации о блоке

Game::Game() :
    fixedDelta_{ 1 / 60.0f },
    window_{ sf::VideoMode(WIDTH, HEIGHT), "Terraria Clone" },
    view_{sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT) },
    player_{ },
    fpsText_{ },
    positionText_{ },
    noclip_{ },
    drawHitBoxes_{ }
{
    resizeWindow();

    window_.setFramerateLimit(144);

    fpsText_.setFont(ResourceManager::getFont());
    fpsText_.setFillColor(sf::Color::Red);
    fpsText_.setCharacterSize(15);
    fpsText_.setString("fps: 0");

    positionText_.setFont(ResourceManager::getFont());
    positionText_.setFillColor(sf::Color::Red);
    positionText_.setCharacterSize(15);
    positionText_.move(0.0f, 16.0f);


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
}

void Game::start() {
    sf::Clock timer;
    sf::Clock fpsTimer;
    int fps{ 0 };
    float accumulator{ 0.0f };

    while (window_.isOpen()) {
        if (fpsTimer.getElapsedTime().asSeconds() > 1.0f) {
            fpsText_.setString("fps: " + std::to_string(fps));
            fps = 0;
            fpsTimer.restart();
        } else {
            fps++;
        }

        sf::Vector2i playerPos{ mapGlobalCoordsToGame(player_.getPosition().x, player_.getHitBox().getGlobalBounds().top) };
        positionText_.setString("x: " + std::to_string(playerPos.x) + " y: " + std::to_string(playerPos.y));

        handleEvents();
        float frameTime{ timer.restart().asSeconds() };
        accumulator += frameTime;
        update();
        while (accumulator >= fixedDelta_) {
            fixedUpdate();
            accumulator -= fixedDelta_;
        }
        render();
    }
}

void Game::handleEvents() {
    sf::Event e{};
    while (window_.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            window_.close();
        }
        if (e.type == sf::Event::Resized) {
            // change game view ratio
            resizeWindow();
        }
    }
    InputHandler::updateStates();
}

void Game::update() {
    if (InputHandler::getMouseButtonState(sf::Mouse::Left) == InputHandler::JUST_PRESSED) {
        window_.setView(view_);
        sf::Vector2f globalCoords{ window_.mapPixelToCoords(sf::Mouse::getPosition(window_)) };
        sf::Vector2i pos{ mapGlobalCoordsToGame(globalCoords) };
        if (math::distanceBetween(mapGlobalCoordsToGame(player_.getPosition()), pos) <= BREAK_PLACE_DISTANCE) {
            const Block* block { World::destroyBlock(pos) };
            if (block) {
                player_.getHotBar().addItem(block->type);
            }
        }
        window_.setView(window_.getDefaultView());
    }

    if (InputHandler::getMouseButtonState(sf::Mouse::Right) == InputHandler::JUST_PRESSED) {
        window_.setView(view_);
        sf::Vector2f globalCoords{ window_.mapPixelToCoords(sf::Mouse::getPosition(window_)) };
        sf::Vector2i pos{ mapGlobalCoordsToGame(globalCoords) };
        if (math::distanceBetween(mapGlobalCoordsToGame(player_.getPosition()), pos) <= BREAK_PLACE_DISTANCE &&
            canPlaceBlock(player_, pos))
        {
            const InventoryCell& cell { player_.getHotBar().getCell(player_.getHeldItem(), 0) };
            if (cell.amount != 0) {
                World::placeBlock(pos, cell.blockType);
                player_.getHotBar().removeItem(player_.getHeldItem(), 0, 1);
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

    player_.updateStates();
}

void Game::fixedUpdate() {
    if (!noclip_) {
        if (player_.getDistanceToGround() < 0.5f) {
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
        player_.moveWithCollide();
    }

    player_.update(fixedDelta_);

    if (!noclip_) {
        player_.horizontalSpeed = 0;
    }

    view_.setCenter(player_.getPosition());
}

void Game::render() {
    window_.clear(sf::Color::White);

    window_.setView(view_);
    World::draw(window_);
    window_.draw(player_);
    if (drawHitBoxes_) {
        window_.draw(player_.getHitBox());
    }

    window_.setView(window_.getDefaultView());
    window_.draw(fpsText_);
    window_.draw(positionText_);

    window_.display();
}

void Game::resizeWindow() {
    float ratio{ static_cast<float>(window_.getSize().x) / static_cast<float>(window_.getSize().y) };
    view_.setSize(VIEW_WIDTH * ratio, VIEW_HEIGHT);
}
