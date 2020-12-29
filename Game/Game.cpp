//
// Created by adjsky on 12/28/20.
//

#include "Game.h"
#include "../InputHandler/InputHandler.h"
#include "../ResourceManager/ResourceManager.h"

#include <iostream>

Game::Game() :
    window{ sf::VideoMode(WIDTH, HEIGHT), "Terraria Clone" },
    view{ sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT) },
    world{},
    fpsText{}
{
    window.setFramerateLimit(144);
    fpsText.setFont(*ResourceManager::getFont());
    fpsText.setFillColor(sf::Color::Red);
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
            float ratio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
            view.setSize(VIEW_WIDTH * ratio, VIEW_HEIGHT);
        }
    }
    InputHandler::updateStates();
}

void Game::update(float delta) {
    view.setCenter(player);
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::A) == InputHandler::STILL_PRESSED) {
        player.x -= 1.0f * delta;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::D) == InputHandler::STILL_PRESSED) {
        player.x += 1.0f * delta;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::W) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::W) == InputHandler::STILL_PRESSED) {
        player.y -= 1.0f * delta;
    }
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::S) == InputHandler::JUST_PRESSED ||
        InputHandler::getKeyboardKeyState(sf::Keyboard::S) == InputHandler::STILL_PRESSED) {
        player.y += 1.0f * delta;
    }
    if (InputHandler::getMouseButtonState(sf::Mouse::Left) == InputHandler::JUST_PRESSED) {
        window.setView(view);
        sf::Vector2f globalCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2i pos = mapGlobalCoordsToGame(globalCoords);
        world.destroyBlock(pos);
        window.setView(window.getDefaultView());
    }
}

void Game::render() {
    window.clear(sf::Color::White);
    window.setView(view);
    window.draw(world);
    window.setView(window.getDefaultView());
    window.draw(fpsText);
    window.display();
}

sf::Vector2i Game::mapGlobalCoordsToGame(sf::Vector2f& globalCoords) {
    sf::Vector2i gameCoords{ static_cast<int>(floor(globalCoords.x / BLOCK_SIZE)), -1 * static_cast<int>(std::floor(globalCoords.y / BLOCK_SIZE)) };
    return gameCoords;
}
