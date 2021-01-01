//
// Created by adjsky on 12/28/20.
//

#ifndef TERRARIA_CLONE_INPUTHANDLER_H
#define TERRARIA_CLONE_INPUTHANDLER_H

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <array>

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
    static States getMouseButtonState(sf::Mouse::Button button);
    static States getKeyboardKeyState(sf::Keyboard::Key key);
    static void updateStates();

private:
    static void updateKeyboardKey(sf::Keyboard::Key key);
    static void updateMouseButton(sf::Mouse::Button button);
    static std::array<KeyFrames, sf::Keyboard::KeyCount> keyboardKeys;
    static std::array<KeyFrames, sf::Mouse::ButtonCount> mouseButtons;
};


#endif //TERRARIA_CLONE_INPUTHANDLER_H