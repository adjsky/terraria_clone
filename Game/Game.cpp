//
// Created by adjsky on 12/28/20.
//

#include "Game.h"
#include "../InputHandler/InputHandler.h"

#include <iostream>

Game::Game() :
    window{ sf::VideoMode(WIDTH, HEIGHT), "Terraria Clone" },
    view{ sf::FloatRect(0, 0, WIDTH, HEIGHT) },
    texture{},
    sprite{}
{
    window.setFramerateLimit(60);

    texture.loadFromFile("../Resources/Spritesheets/block_sprites.png", sf::IntRect(96, 0, 96, 96));
    sprite.setTexture(texture);
    sprite.move(200.0f, 300.0f);
}

void Game::start() {
    sf::Clock timer;
    while (window.isOpen()) {
        handleEvents();
        update(timer.restart());
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

void Game::update(sf::Time delta) {
//    if (InputHandler::getMouseButtonState(sf::Mouse::Left) == InputHandler::States::JUST_PRESSED) {
//        view.setSize(view.getSize().x * 1.1f, view.getSize().y * 1.1f);
//    }
//    if (InputHandler::getMouseButtonState(sf::Mouse::Right) == InputHandler::States::JUST_PRESSED) {
//        view.setSize(view.getSize().x / 1.1f, view.getSize().y / 1.1f);
//    }
}

void Game::render() {
    window.clear(sf::Color::White);
    window.setView(view);
    window.draw(sprite);
    window.setView(window.getDefaultView());
    window.display();
}
