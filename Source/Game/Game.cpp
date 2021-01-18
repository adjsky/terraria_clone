//
// Created by adjsky on 12/28/20.
//

#include <iostream>

#include "Game.h"
#include "../InputHandler/InputHandler.h"

Game::Game(const sf::ContextSettings& context) :
    fixedDelta_{ 1 / 60.0f },
    window_{ sf::VideoMode(WIDTH, HEIGHT), "Terraria Clone", sf::Style::Default, context },
    gui_{ window_ },
    currentGameSession{ nullptr }
{
    resizeWindow();
    window_.setFramerateLimit(144);

    currentGameSession = std::make_unique<GameSession>(window_, gui_);
}

void Game::start() {
    sf::Clock timer;
    float accumulator{ 0.0f };

    while (window_.isOpen()) {
        handleEvents();

        float frameTime{ timer.restart().asSeconds() };
        if (currentGameSession) currentGameSession->update(frameTime);
        accumulator += frameTime;
        while (accumulator >= fixedDelta_) {
            if (currentGameSession) {
                currentGameSession->fixedUpdate(fixedDelta_);
            }
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
        gui_.handleEvent(e);
    }
    InputHandler::updateStates();
}

void Game::render() {
    window_.clear(sf::Color::White);

    if (currentGameSession) currentGameSession->render();

    window_.setView(window_.getDefaultView());
    gui_.draw();

    window_.display();
}

void Game::resizeWindow() {
//    float ratio{ static_cast<float>(window_.getSize().x) / static_cast<float>(window_.getSize().y) };
//    view_.setSize(std::floor(VIEW_WIDTH * ratio), VIEW_HEIGHT);
}