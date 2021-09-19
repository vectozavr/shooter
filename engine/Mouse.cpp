//
// Created by Иван Ильин on 19.09.2021.
//

#include "Mouse.h"
#include "utils/Time.h"

Point4D Mouse::getMousePosition() const {
    sf::Vector2<int> pos = sf::Mouse::getPosition(*_window);
    return Point4D(pos.x, pos.y, 0, 0);
}

Point4D Mouse::getMouseDisplacement() const {
    sf::Vector2<int> mousePos = sf::Mouse::getPosition(*_window);
    sf::Vector2<int> center = sf::Vector2<int>(_window->getSize().x/2, _window->getSize().y/2);

    sf::Vector2<int> disp = mousePos - center;
    //setMouseInCenter();
    return Point4D(disp.x, disp.y, 0, 0);
}

void Mouse::setMouseInCenter() const {
    sf::Mouse::setPosition({ static_cast<int>(_window->getSize().x / 2), static_cast<int>(_window->getSize().y / 2) }, *_window);
}

bool Mouse::isButtonPressed(sf::Mouse::Button button) {
    return sf::Mouse::isButtonPressed(button);
}

bool Mouse::isButtonTapped(sf::Mouse::Button button) {
    if (!Mouse::isButtonPressed(button))
        return false;

    if(_tappedButtons.count(button) == 0) {
        _tappedButtons.emplace(button, Time::time());
        return true;
    } else if((Time::time() - _tappedButtons[button]) > 0.2) {
        _tappedButtons[button] = Time::time();
        return true;
    }
    return false;
}

void Mouse::setWindow(std::shared_ptr<sf::RenderWindow> window) {
    _window = window;
}

void Mouse::setMouseCursorVisible(bool visible) {
    _window->setMouseCursorVisible(visible);
}
