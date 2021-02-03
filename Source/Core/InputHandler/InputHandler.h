//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_INPUTHANDLER_H
#define TERRARIA_CLONE_INPUTHANDLER_H

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <array>

// @TODO
// Объедить клавиатуру и мышь

struct KeyFrames {
    bool lastFrame;
    bool currentFrame;
};

class InputHandler {
public:
    enum States {
        JUST_PRESSED,
        JUST_RELEASED,
        STILL_PRESSED,
        STILL_RELEASED
    };

public:
    InputHandler();

    States getMouseButtonState(sf::Mouse::Button button);
    States getKeyboardKeyState(sf::Keyboard::Key key);
    void updateStates();

private:
    void updateKeyboardKey(sf::Keyboard::Key key);
    void updateMouseButton(sf::Mouse::Button button);
    std::array<KeyFrames, sf::Keyboard::KeyCount> keyboardKeys_;
    std::array<KeyFrames, sf::Mouse::ButtonCount> mouseButtons_;
};

#endif //TERRARIA_CLONE_INPUTHANDLER_H