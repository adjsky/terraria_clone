//
// Created by adjsky on 12/28/20.
//

#include "InputHandler.h"

std::array<KeyFrames, sf::Keyboard::KeyCount> InputHandler::keyboardKeys{};
std::array<KeyFrames, sf::Mouse::ButtonCount> InputHandler::mouseButtons{};

void InputHandler::updateMouseButton(sf::Mouse::Button button) {
    mouseButtons[button].lastFrame = mouseButtons[button].currentFrame;
    mouseButtons[button].currentFrame = sf::Mouse::isButtonPressed(button);
}

InputHandler::States InputHandler::getMouseButtonState(sf::Mouse::Button button) {
    if (mouseButtons[button].lastFrame) {
        if (mouseButtons[button].currentFrame) {
            return States::STILL_PRESSED;
        }
        else {
            return States::JUST_RELEASED;
        }
    }
    else {
        if (mouseButtons[button].currentFrame) {
            return States::JUST_PRESSED;
        }
        else {
            return States::STILL_RELEASED;
        }
    }
}

void InputHandler::updateKeyboardKey(sf::Keyboard::Key key) {
    keyboardKeys[key].lastFrame = keyboardKeys[key].currentFrame;
    keyboardKeys[key].currentFrame = sf::Keyboard::isKeyPressed(key);
}

InputHandler::States InputHandler::getKeyboardKeyState(sf::Keyboard::Key key) {
    if (keyboardKeys[key].lastFrame) {
        if (keyboardKeys[key].currentFrame) {
            return States::STILL_PRESSED;
        }
        else {
            return States::JUST_RELEASED;
        }
    }
    else {
        if (keyboardKeys[key].currentFrame) {
            return States::JUST_PRESSED;
        }
        else {
            return States::STILL_RELEASED;
        }
    }
}

void InputHandler::updateStates() {
    for (int k = sf::Keyboard::A; k < sf::Keyboard::KeyCount; k++) {
        updateKeyboardKey(static_cast<sf::Keyboard::Key>(k));
    }
    for (int k = sf::Mouse::Left; k < sf::Mouse::ButtonCount; k++) {
        updateMouseButton(static_cast<sf::Mouse::Button>(k));
    }
}