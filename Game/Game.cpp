//
// Created by adjsky on 12/28/20.
//

#include <iostream>

#include "Game.h"
#include "../InputHandler/InputHandler.h"
#include "../ResourceManager/ResourceManager.h"
#include "../World/World.h"
#include "../Physics/Collisions.h"

Game::Game() :
    window_{ sf::VideoMode(WIDTH, HEIGHT), "Terraria Clone" },
    view_{sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT) },
    fpsText_{},
    player_{ ResourceManager::getTexture(ResourceManager::PLAYER)  },
    fixedDelta{ 1 / 50.0f }
{
    window_.setFramerateLimit(144);
    fpsText_.setFont(*ResourceManager::getFont());
    fpsText_.setFillColor(sf::Color::Red);

    player_.setPeriod(0.2f);
    player_.constructHitBox();
    player_.move(0.0f, -260.0f * BLOCK_SIZE);

    player_.addAnimationFrame(AnimatedPlayer::STAND, sf::IntRect(28, 14, 74, 106));
    player_.addAnimationFrame(AnimatedPlayer::STAND, sf::IntRect(762, 154, 74, 106));
    player_.setAnimation(AnimatedPlayer::STAND);
    player_.setTextureRect(sf::IntRect(28, 14, 74, 106));
}

void Game::start() {
    sf::Clock timer;
    float accumulator = 0.0f;

    while (window_.isOpen()) {
        handleEvents();
        float frameTime = timer.restart().asSeconds();
        accumulator += frameTime;
        update();
        while (accumulator >= fixedDelta) {
            fixedUpdate();
            accumulator -= fixedDelta;
        }
        render();
    }
}

void Game::handleEvents() {
    sf::Event e;
    while (window_.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            window_.close();
        }
        if (e.type == sf::Event::Resized) {
            // change game view ratio
            float ratio = static_cast<float>(window_.getSize().x) / static_cast<float>(window_.getSize().y);
            view_.setSize(VIEW_WIDTH * ratio, VIEW_HEIGHT);
        }
    }
    InputHandler::updateStates();
}

void Game::update() {
    if (InputHandler::getMouseButtonState(sf::Mouse::Left) == InputHandler::JUST_PRESSED ||
        InputHandler::getMouseButtonState(sf::Mouse::Left) == InputHandler::STILL_PRESSED) {
        window_.setView(view_);
        sf::Vector2f globalCoords = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        sf::Vector2i pos = mapGlobalCoordsToGame(globalCoords);
        World::destroyBlock(pos);
        window_.setView(window_.getDefaultView());
    }
    if (InputHandler::getMouseButtonState(sf::Mouse::Right) == InputHandler::JUST_PRESSED ||
        InputHandler::getMouseButtonState(sf::Mouse::Right) == InputHandler::STILL_PRESSED) {
        window_.setView(view_);
        sf::Vector2f globalCoords = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        sf::Vector2i pos = mapGlobalCoordsToGame(globalCoords);
        World::placeBlock(pos);
        window_.setView(window_.getDefaultView());
    }
}

void Game::fixedUpdate() {
    player_.isOnGround = checkFootBlocks(player_);
    if (!player_.isOnGround) {
        player_.verticalSpeed += 0.05f;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::STILL_PRESSED) {
        if (player_.isOnGround) {
            player_.verticalSpeed = -2.0f;
            player_.isOnGround = false;
        }
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::STILL_PRESSED) {
        player_.horizontalSpeed = -2.0f;
        player_.setDirection(Player::MoveDirection::LEFT);
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::STILL_PRESSED) {
        player_.horizontalSpeed = 2.0f;
        player_.setDirection(Player::MoveDirection::RIGHT);
    }

    if (!checkPossibleVerticalCollision(player_)) {
        player_.move(0, player_.verticalSpeed);
    }
    if (!checkPossibleHorizontalCollision(player_)) {
        player_.move(player_.horizontalSpeed, 0);
    }

    player_.horizontalSpeed = 0;

    view_.setCenter(player_.getPosition());
    player_.update(fixedDelta);
}

void Game::render() {
    window_.clear(sf::Color::White);

    window_.setView(view_);
    World::draw(window_);
    window_.draw(player_);

    window_.setView(window_.getDefaultView());
    window_.draw(fpsText_);

    window_.display();
}


