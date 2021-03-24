//
// Created by adjsky on 03.02.2021.
//

#include "Engine.h"

std::unique_ptr<InputHandler> Engine::inputHandler_{};

std::unique_ptr<ResourceManager> Engine::resourceManager_{};

std::unique_ptr<entt::dispatcher> Engine::eventSystem_{};

std::unique_ptr<DatabaseManager> Engine::databaseManager_{};

std::unique_ptr<sf::RenderWindow> Engine::window_{};

std::unique_ptr<Interface> Engine::interface_{};

void Engine::init()
{
    inputHandler_ = std::make_unique<InputHandler>();
    resourceManager_ = std::make_unique<ResourceManager>();
    eventSystem_ = std::make_unique<entt::dispatcher>();
    databaseManager_ = std::make_unique<DatabaseManager>();

    sf::ContextSettings context{};
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(WIDTH, HEIGHT), "Terraria Clone", sf::Style::Default, context);
    window_->setFramerateLimit(144);

    interface_ = std::make_unique<Interface>(*window_);
}

InputHandler* Engine::getInputHandler()
{
    return inputHandler_.get();
}

ResourceManager* Engine::getResourceManager()
{
    return resourceManager_.get();
}

entt::dispatcher* Engine::getEventSystem()
{
    return eventSystem_.get();
}

DatabaseManager* Engine::getDatabaseManager()
{
    return databaseManager_.get();
}

Interface* Engine::getInterface()
{
    return interface_.get();
}

sf::RenderWindow* Engine::getWindow()
{
    return window_.get();
}
