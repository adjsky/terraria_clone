//
// Created by adjsky on 03.02.2021.
//

#include "Engine.h"

std::unique_ptr<InputHandler> Engine::inputHandler_{};
std::unique_ptr<ResourceManager> Engine::resourceManager_{};
std::unique_ptr<entt::dispatcher> Engine::eventSystem_{};

void Engine::init() {
    inputHandler_ = std::make_unique<InputHandler>();
    resourceManager_ = std::make_unique<ResourceManager>();
    eventSystem_ = std::make_unique<entt::dispatcher>();
}

InputHandler* Engine::getInputHandler() {
    return inputHandler_.get();
}

ResourceManager* Engine::getResourceManager() {
    return resourceManager_.get();
}

entt::dispatcher* Engine::getEventSystem() {
    return eventSystem_.get();
}