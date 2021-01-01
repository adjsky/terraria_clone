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
    player_{ ResourceManager::getTexture(ResourceManager::PLAYER)  }
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
}

void Game::start() {
    sf::Clock timer;
    float fpsTimer = 0;
    float fps = 0;
    while (window_.isOpen()) {
        float delta = timer.restart().asMilliseconds();
        fpsTimer += delta;
        if (fpsTimer > 1000) {
            fpsText_.setString("fps: " + std::to_string(fps));
            fpsTimer = 0;
            fps = 0;
        }
        else {
            fps += 1;
        }
        handleEvents();
        update(delta);
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

void Game::update(float delta) {
    updatePlayer(delta);
    handleClick();
    updateCamera();
}

void Game::updatePlayer(float delta) {
    // vertical
    if (checkBottomCollision(player_)) {
        player_.isOnGround = true;
    }
    else {
        player_.isOnGround = false;
    }

    if (!player_.isOnGround) {
        player_.verticalSpeed += G * delta / 12500.0f;
    } else {
        player_.verticalSpeed = 0;
    }

    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::JUST_PRESSED) {
        if (player_.isOnGround) {
            player_.verticalSpeed = -1.0f;
            player_.isOnGround = false;
        }
    }

    if (!checkTopCollision(player_)){
        player_.move(0, player_.verticalSpeed);
    }
    else {
        player_.verticalSpeed = 0;
    }

    // horizontal
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::STILL_PRESSED) {
        player_.horizontalSpeed = -0.2f * delta;
        if (!checkLeftSideCollision(player_)) {
            player_.move(player_.horizontalSpeed, 0);
        }
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::STILL_PRESSED) {
        player_.horizontalSpeed = 0.2f * delta;
        if (!checkRightSideCollision(player_)) {
            player_.move(player_.horizontalSpeed, 0);
        }
    }

    while (checkRightSideCollision(player_)) {
        player_.move(-0.1f, 0.0f);
    }

    while (checkLeftSideCollision(player_)) {
        player_.move(0.1f, 0.0f);
    }

    player_.update(delta);

    player_.horizontalSpeed = 0;
}

void Game::handleClick() {
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

void Game::updateCamera() {
    view_.setCenter(player_.getPosition());
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
