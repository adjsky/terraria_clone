//
// Created by adjsky on 03.02.2021.
//

#ifndef TERRARIA_CLONE_ENGINE_H
#define TERRARIA_CLONE_ENGINE_H

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../Interface/Interface.h"
#include "InputHandler/InputHandler.h"
#include "ResourceManager/ResourceManager.h"
#include "../Entities/Items/DatabaseManager.h"
#include "../Util/entt.hpp"

class Engine
{
public:
    static void init();

    static InputHandler* getInputHandler();
    static ResourceManager* getResourceManager();
    static entt::dispatcher* getEventSystem();
    static DatabaseManager* getDatabaseManager();
    static Interface* getInterface();
    static sf::RenderWindow* getWindow();

private:
    static std::unique_ptr<entt::dispatcher> eventSystem_;
    static std::unique_ptr<InputHandler> inputHandler_;
    static std::unique_ptr<ResourceManager> resourceManager_;
    static std::unique_ptr<DatabaseManager> databaseManager_;
    static std::unique_ptr<sf::RenderWindow> window_;
    static std::unique_ptr<Interface> interface_;
};


#endif //TERRARIA_CLONE_ENGINE_H
