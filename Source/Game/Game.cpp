//
// Created by adjsky on 12/28/20.
//

#include "Game.h"
#include "../Core/Engine.h"
#include "../Util/Serialization/GameSerialization.h"

Game::Game(const sf::ContextSettings& context) :
        fixedDelta_{ 1 / 60.0f },
        window_{ sf::VideoMode(WIDTH, HEIGHT), "Terraria Clone", sf::Style::Default, context },
        gui_{ window_ },
        currentGameSession_{ nullptr },
        gameLogic_{ },
        camera_{ sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT) },
        drawHitBoxes_{ false },
        paused_{ false }
{
    resizeWindow();
    window_.setFramerateLimit(144);

//    gui_.consoleEntered.connect([this](const std::string& data){
//        consoleHandler_.process(data);
//    });
}

void Game::start() {
    sf::Clock timer;
    float accumulator{ 0.0f };

    while (window_.isOpen()) {
        handleEvents();

        float frameTime{ timer.restart().asSeconds() };
        if (currentGameSession_) gameLogic_.update(frameTime);
        accumulator += frameTime;
        while (accumulator >= fixedDelta_) {
            if (currentGameSession_) {
                gameLogic_.fixedUpdate(fixedDelta_);
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
            close();
        }
        if (e.type == sf::Event::Resized) {
            // change game view ratio
            resizeWindow();
        }
        gui_.handleEvent(e);
    }
    Engine::getInputHandler()->updateStates();
}

void Game::close() {
    if (currentGameSession_)
        GameSerialization::saveGame(*currentGameSession_);
    window_.close();
}

GameSession* Game::getGameSession() {
    return currentGameSession_.get();
}

GameSession* Game::createGameSession() {
    currentGameSession_ = std::make_unique<GameSession>();
    return currentGameSession_.get();
}

void Game::deleteGameSession() {
    currentGameSession_ = nullptr;
}

Interface& Game::getInterface() {
    return gui_;
}

sf::RenderWindow& Game::getWindow() {
    return window_;
}

sf::View& Game::getCamera() {
    return camera_;
}

void Game::drawHitBoxes(bool condition) {
    drawHitBoxes_ = condition;
}

void Game::pause(bool condition) {
    paused_ = condition;
}

bool Game::isPaused() const {
    return paused_;
}

void Game::render() {
    window_.clear(sf::Color::White);

    if (currentGameSession_) {
        window_.setView(camera_);
        currentGameSession_->getWorld().draw(window_);
        window_.draw( currentGameSession_->getPlayer());
        if (drawHitBoxes_) {
            window_.draw( currentGameSession_->getPlayer().getHitBox());
        }
        window_.setView(window_.getDefaultView());
    }

    window_.setView(window_.getDefaultView());
    gui_.draw();

    window_.display();
}

void Game::resizeWindow() {
//    float ratio{ static_cast<float>(window_.getSize().x) / static_cast<float>(window_.getSize().y) };
//    view_.setSize(std::floor(VIEW_WIDTH * ratio), VIEW_HEIGHT);
}