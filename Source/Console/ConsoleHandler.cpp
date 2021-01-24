//
// Created by adjsky on 20.01.2021.
//

#include <iostream>

#include "ConsoleHandler.h"

ConsoleHandler::ConsoleHandler(GameSession* gameSession) :
    gameSession_{ gameSession }
{

}

void ConsoleHandler::setGameSession(GameSession* gameSession) {
    gameSession_ = gameSession;
}

void ConsoleHandler::process(const std::string& data) {
    std::cout << data << '\n';
}
