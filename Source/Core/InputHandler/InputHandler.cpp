//
// Created by adjsky on 12/28/20.
//

#include "InputHandler.h"


InputHandler::InputHandler() :
    keyboardKeys_{ },
    mouseButtons_{ }
{

}

void InputHandler::updateMouseButton(sf::Mouse::Button button)
{
    mouseButtons_[button].lastFrame = mouseButtons_[button].currentFrame;
    mouseButtons_[button].currentFrame = sf::Mouse::isButtonPressed(button);
}

InputHandler::States InputHandler::getMouseButtonState(sf::Mouse::Button button)
{
    if (mouseButtons_[button].lastFrame) {
        if (mouseButtons_[button].currentFrame) {
            return States::STILL_PRESSED;
        }
        else {
            return States::JUST_RELEASED;
        }
    }
    else {
        if (mouseButtons_[button].currentFrame) {
            return States::JUST_PRESSED;
        }
        else {
            return States::STILL_RELEASED;
        }
    }
}

void InputHandler::updateKeyboardKey(sf::Keyboard::Key key)
{
    keyboardKeys_[key].lastFrame = keyboardKeys_[key].currentFrame;
    keyboardKeys_[key].currentFrame = sf::Keyboard::isKeyPressed(key);
}

InputHandler::States InputHandler::getKeyboardKeyState(sf::Keyboard::Key key)
{
    if (keyboardKeys_[key].lastFrame) {
        if (keyboardKeys_[key].currentFrame) {
            return States::STILL_PRESSED;
        }
        else {
            return States::JUST_RELEASED;
        }
    }
    else {
        if (keyboardKeys_[key].currentFrame) {
            return States::JUST_PRESSED;
        }
        else {
            return States::STILL_RELEASED;
        }
    }
}

void InputHandler::updateStates()
{
    for (int k = sf::Keyboard::A; k < sf::Keyboard::KeyCount; k++) {
        updateKeyboardKey(static_cast<sf::Keyboard::Key>(k));
    }
    for (int k = sf::Mouse::Left; k < sf::Mouse::ButtonCount; k++) {
        updateMouseButton(static_cast<sf::Mouse::Button>(k));
    }
}