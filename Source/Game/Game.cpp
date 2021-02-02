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
    currentGameSession{ nullptr },
    shouldUpdate_{ true },
    consoleHandler_{ nullptr }
{
    resizeWindow();
    window_.setFramerateLimit(144);

    currentGameSession = std::make_unique<GameSession>(window_, gui_);
    consoleHandler_.setGameSession(currentGameSession.get());

    gui_.consoleEnterSignal.connect([this](const std::string& data){
        consoleHandler_.process(data);
    });
    gui_.hotBarCellPressed.connect([this](int x){
        if (gui_.inventoryIsOpen()) {
            Inventory::Cell cell{ currentGameSession->getPlayer().getHotBar().getCell(x, 0) };
            if (currentGameSession->getPlayer().hasAttachedItem) {
                bool swapped { currentGameSession->getPlayer().getHotBar().setItem(currentGameSession->getPlayer().attachedItem, x, 0) };
                if (swapped) {
                    currentGameSession->getPlayer().attachedItem = cell;
                    gui_.updateAttachedItem(currentGameSession->getPlayer(), true);
                }
                else {
                    currentGameSession->getPlayer().hasAttachedItem = false;
                    gui_.updateAttachedItem(currentGameSession->getPlayer());
                }
                gui_.updateHotBar(currentGameSession->getPlayer());
            }
            else {
                if (cell.amount) {
                    currentGameSession->getPlayer().attachedItem = cell;
                    currentGameSession->getPlayer().hasAttachedItem = true;
                    currentGameSession->getPlayer().getHotBar().removeItem(x, 0);
                    gui_.updateHotBar(currentGameSession->getPlayer());
                    gui_.updateAttachedItem(currentGameSession->getPlayer());
                }
            }
        }
    });

    gui_.backpackCellPressed.connect([this](int x, int y){
        if (gui_.inventoryIsOpen()) {
            Inventory::Cell cell{ currentGameSession->getPlayer().getBackpack().getCell(x, y) };
            if (currentGameSession->getPlayer().hasAttachedItem) {
                bool swapped{ currentGameSession->getPlayer().getBackpack().setItem(currentGameSession->getPlayer().attachedItem, x, y) };
                if (swapped) {
                    currentGameSession->getPlayer().attachedItem = cell;
                    gui_.updateAttachedItem(currentGameSession->getPlayer(), true);
                }
                else {
                    currentGameSession->getPlayer().hasAttachedItem = false;
                    gui_.updateAttachedItem(currentGameSession->getPlayer());
                }
                gui_.updateInventory(currentGameSession->getPlayer());
            }
            else {
                if (cell.amount) {
                    currentGameSession->getPlayer().attachedItem = cell;
                    currentGameSession->getPlayer().hasAttachedItem = true;
                    currentGameSession->getPlayer().getBackpack().removeItem(x, y);
                    gui_.updateInventory(currentGameSession->getPlayer());
                    gui_.updateAttachedItem(currentGameSession->getPlayer());
                }
            }
        }
    });
}

void Game::start() {
    sf::Clock timer;
    float accumulator{ 0.0f };

    while (window_.isOpen()) {
        handleEvents();

        float frameTime{ timer.restart().asSeconds() };
        if (currentGameSession && shouldUpdate_) currentGameSession->update(frameTime);
        accumulator += frameTime;
        while (accumulator >= fixedDelta_) {
            if (currentGameSession && shouldUpdate_) {
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
    if (InputHandler::getKeyboardKeyState(sf::Keyboard::Slash) == InputHandler::JUST_PRESSED) {
        gui_.showConsole();
        shouldUpdate_ = !shouldUpdate_;
    }
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