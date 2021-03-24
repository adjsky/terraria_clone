//
// Created by adjsky on 12/28/20.
//

#include "Game.h"
#include "../Core/Engine.h"
#include "../Util/Serialization/GameSerialization.h"

Game::Game() :
    fixedDelta_{ 1 / 60.0f },
    currentGameSession_{ nullptr },
    gameLogic_{ *this },
    camera_{ sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT) },
    drawHitBoxes_{ false },
    paused_{ false }
{
//    gui_.consoleEntered.connect([this](const std::string& data){
//        consoleHandler_.process(data);
//    });
}

void Game::start()
{
    sf::Clock timer;
    float accumulator{ 0.0f };

    while (Engine::getWindow()->isOpen()) {
        handleEvents();

        float frameTime{ timer.restart().asSeconds() };
        if (currentGameSession_) { gameLogic_.update(frameTime); }
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

void Game::handleEvents()
{
    sf::Event e{ };
    while (Engine::getWindow()->pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            close();
        }
        Engine::getInterface()->handleEvent(e);
    }
    Engine::getInputHandler()->updateStates();
}

void Game::close()
{
    if (currentGameSession_) {
        GameSerialization::saveGame(*currentGameSession_);
    }
    Engine::getWindow()->close();
}

GameSession* Game::getGameSession()
{
    return currentGameSession_.get();
}

GameSession* Game::createGameSession()
{
    currentGameSession_ = std::make_unique<GameSession>();
    paused_ = false;
    drawHitBoxes_ = false;
    return currentGameSession_.get();
}

void Game::deleteGameSession()
{
    currentGameSession_ = nullptr;
}

sf::View& Game::getCamera()
{
    return camera_;
}

void Game::drawHitBoxes(bool condition)
{
    drawHitBoxes_ = condition;
}

void Game::pause(bool condition)
{
    paused_ = condition;
}

bool Game::isPaused() const
{
    return paused_;
}

void Game::render()
{
    auto* window{ Engine::getWindow() };
    window->clear(sf::Color::White);

    if (currentGameSession_) {
        window->setView(camera_);
        currentGameSession_->getWorld().draw(*window);
        window->draw(currentGameSession_->getPlayer());
        if (drawHitBoxes_) {
            window->draw(currentGameSession_->getPlayer().getHitBox());
        }
        window->setView(window->getDefaultView());
    }

    window->setView(window->getDefaultView());
    Engine::getInterface()->draw();

    window->display();
}