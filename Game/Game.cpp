//
// Created by adjsky on 12/28/20.
//

#include <iostream>

#include "Game.h"
#include "../InputHandler/InputHandler.h"
#include "../ResourceManager/ResourceManager.h"
#include "../World/World.h"

Game::Game() :
        window_{ sf::VideoMode(WIDTH, HEIGHT), "Terraria Clone" },
        view_{sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT) },
        fpsText_{},
        positionText_{},
        player_{ ResourceManager::getTexture(ResourceManager::PLAYER)  },
        fixedDelta{ 1 / 60.0f },
        noclip_{ }
{
    window_.setFramerateLimit(144);

    fpsText_.setFont(*ResourceManager::getFont());
    fpsText_.setFillColor(sf::Color::Red);
    fpsText_.setCharacterSize(15);
    fpsText_.setString("fps: 0");

    positionText_.setFont(*ResourceManager::getFont());
    positionText_.setFillColor(sf::Color::Red);
    positionText_.setCharacterSize(15);
    positionText_.move(0.0f, 16.0f);

    player_.setPeriod(0.2f);
    player_.constructHitBox();

    player_.move(0.0f, -61.0f * BLOCK_SIZE);

    player_.addAnimationFrame(AnimatedPlayer::STAND, sf::IntRect(28, 14, PLAYER_WIDTH, PLAYER_HEIGHT));
    player_.addAnimationFrame(AnimatedPlayer::MOVING, sf::IntRect(28, 14, PLAYER_WIDTH, PLAYER_HEIGHT));
    player_.setTextureRect(sf::IntRect(28, 14, 74, 106));
    player_.setScale((float)BLOCK_SIZE / PLAYER_WIDTH,(float)BLOCK_SIZE / PLAYER_HEIGHT * 2);
}

void Game::start() {
    sf::Clock timer;
    sf::Clock fpsTimer;
    int fps = 0;
    float accumulator = 0.0f;

    while (window_.isOpen()) {
        if (fpsTimer.getElapsedTime().asSeconds() > 1.0f) {
            fpsText_.setString("fps: " + std::to_string(fps));
            fps = 0;
            fpsTimer.restart();
        } else {
            fps++;
        }

        sf::Vector2i playerPos = mapGlobalCoordsToGame(player_.getPosition().x, player_.getHitboxBounds().top);
        positionText_.setString("x: " + std::to_string(playerPos.x) + " y: " + std::to_string(playerPos.y));

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
    if (InputHandler::getMouseButtonState(sf::Mouse::Left) == InputHandler::JUST_PRESSED) {
        window_.setView(view_);
        sf::Vector2f globalCoords = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        sf::Vector2i pos = mapGlobalCoordsToGame(globalCoords);
        World::destroyBlock(pos);
        window_.setView(window_.getDefaultView());
    }

    if (InputHandler::getMouseButtonState(sf::Mouse::Right) == InputHandler::JUST_PRESSED) {
        window_.setView(view_);
        sf::Vector2f globalCoords = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        sf::Vector2i pos = mapGlobalCoordsToGame(globalCoords);
        World::placeBlock(pos);
        window_.setView(window_.getDefaultView());
    }

    if (InputHandler::getKeyboardKeyState(sf::Keyboard::X) == InputHandler::JUST_PRESSED) {
        noclip_ = !noclip_;
        player_.verticalSpeed = 0;
    }

    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Tilde) == InputHandler::JUST_PRESSED) {
        player_.drawHitbox(!player_.hitboxIsDrawn());
    }
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
            player_.verticalSpeed += 0.05f;
        }
    }

    bool moved = false;
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::STILL_PRESSED) {
        if (player_.isOnGround && !noclip_) {
            player_.verticalSpeed = -2.0f;
            player_.isOnGround = false;
        }
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::STILL_PRESSED) {
        if (noclip_) {
            player_.move(-7.0f, 0.0f);
        }
        else {
            player_.horizontalSpeed = -2.0f;
        }
        player_.setAnimation(AnimatedPlayer::MOVING);
        player_.setAnimationDirection(Player::LEFT);
        moved = true;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::STILL_PRESSED) {
        if (noclip_) {
            player_.move(7.0f, 0.0f);
        }
        else {
            player_.horizontalSpeed = 2.0f;
        }
        player_.setAnimation(AnimatedPlayer::MOVING);
        player_.setAnimationDirection(Player::RIGHT);
        moved = true;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::W) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::W) == InputHandler::STILL_PRESSED) {
        if (noclip_) {
            player_.move(0.0f, -5.0f);
        }
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::S) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::S) == InputHandler::STILL_PRESSED) {
        if (noclip_) {
            player_.move(0.0f, 5.0f);
        }
    }

    if (!moved) {
        player_.setAnimation(AnimatedPlayer::STAND);
    }

    if (!noclip_) {
        player_.moveWithCollide();
    }

    player_.update(fixedDelta);

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

    window_.setView(window_.getDefaultView());
    window_.draw(fpsText_);
    window_.draw(positionText_);

    window_.display();
}


