//
// Created by adjsky on 20.01.2021.
//

#ifndef TERRARIA_CLONE_CONSOLEHANDLER_H
#define TERRARIA_CLONE_CONSOLEHANDLER_H

#include "../Game/GameSession.h"

class ConsoleHandler {
public:
    explicit ConsoleHandler(GameSession* gameSession);
    void setGameSession(GameSession* gameSession);

    void process(const std::string& data);

private:
    GameSession* gameSession_;
};


#endif //TERRARIA_CLONE_CONSOLEHANDLER_H
