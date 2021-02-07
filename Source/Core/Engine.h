//
// Created by adjsky on 03.02.2021.
//

#ifndef TERRARIA_CLONE_ENGINE_H
#define TERRARIA_CLONE_ENGINE_H

#include <memory>

#include "InputHandler/InputHandler.h"
#include "ResourceManager/ResourceManager.h"
#include "../Util/entt.hpp"

class Engine {
public:
    static void init();

    static InputHandler* getInputHandler();
    static ResourceManager* getResourceManager();
    static entt::dispatcher* getEventSystem();

private:
    static std::unique_ptr<entt::dispatcher> eventSystem_;
    static std::unique_ptr<InputHandler> inputHandler_;
    static std::unique_ptr<ResourceManager> resourceManager_;
};


#endif //TERRARIA_CLONE_ENGINE_H