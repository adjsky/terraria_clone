//
// Created by adjsky on 12/28/20.
//

#include "Game.h"
#include "../InputHandler/InputHandler.h"
#include "../ResourceManager/ResourceManager.h"
#include "../World/World.h"
#include "../Physics/Collisions.h"

#include <iostream>

Game::Game() :
    window{ sf::VideoMode(WIDTH, HEIGHT), "Terraria Clone" },
    view{ sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT) },
    fpsText{},
    player{ *ResourceManager::getTexture(ResourceManager::PLAYER), sf::IntRect(126, 12, 90, 124)  }
{
    window.setFramerateLimit(144);
    fpsText.setFont(*ResourceManager::getFont());
    fpsText.setFillColor(sf::Color::Red);
    player.move(0.0f, -256.0f * BLOCK_SIZE);
}

void Game::start() {
    sf::Clock timer;
    float fpsTimer = 0;
    float fps = 0;
    while (window.isOpen()) {
        float delta = timer.restart().asMilliseconds();
        fpsTimer += delta;
        if (fpsTimer > 1000) {
            fpsText.setString("fps: " + std::to_string(fps));
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
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            window.close();
        }
        if (e.type == sf::Event::Resized) {
            // change game view ratio
            float ratio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
            view.setSize(VIEW_WIDTH * ratio, VIEW_HEIGHT);
        }
    }
    InputHandler::updateStates();
}

void Game::update(float delta) {
    updatePlayer(delta);
    handleClick();
    updateCamera();
}

void Game::render() {
    window.clear(sf::Color::White);

    window.setView(view);
    World::draw(window);
    window.draw(player);

    window.setView(window.getDefaultView());
    window.draw(fpsText);

    window.display();
}

void Game::updatePlayer(float delta) {
    // vertical
    if (checkBottomCollision(player)) {
        player.isOnGround = true;
    }
    else {
        player.isOnGround = false;
    }

    if (!player.isOnGround) {
        player.verticalSpeed += G * delta / 1250.0f;
    } else {
        player.verticalSpeed = 0;
    }

    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Space) == InputHandler::JUST_PRESSED) {
        if (player.isOnGround) {
            std::cout << player.verticalSpeed << '\n';
            player.verticalSpeed = -6.0f;
            player.isOnGround = false;
            std::cout << player.verticalSpeed << '\n';
        }
    }

    player.move(0, player.verticalSpeed);

    // horizontal
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::STILL_PRESSED) {
        player.horizontalSpeed = -0.6f * delta;
        if (!checkLeftSideCollision(player)) {
            player.move(player.horizontalSpeed, 0);
        }
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::STILL_PRESSED) {
        player.horizontalSpeed = 0.6f * delta;
        if (!checkRightSideCollision(player)) {
            player.move(player.horizontalSpeed, 0);
        }
    }
    player.horizontalSpeed = 0;
}

void Game::handleClick() {
    if (InputHandler::getMouseButtonState(sf::Mouse::Left) == InputHandler::JUST_PRESSED) {
        window.setView(view);
        sf::Vector2f globalCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2i pos = mapGlobalCoordsToGame(globalCoords);
        World::destroyBlock(pos);
        window.setView(window.getDefaultView());
    }
    if (InputHandler::getMouseButtonState(sf::Mouse::Right) == InputHandler::JUST_PRESSED) {
        window.setView(view);
        sf::Vector2f globalCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2i pos = mapGlobalCoordsToGame(globalCoords);
        World::placeBlock(pos);
        window.setView(window.getDefaultView());
    }
}

void Game::updateCamera() {
    view.setCenter(player.getPosition());
}
