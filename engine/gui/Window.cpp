//
// Created by Иван Ильин on 26.03.2021.
//

#include "Window.h"

#include <utility>
#include "../ResourceManager.h"

void Window::addButton(int x, int y, int w, int h, std::function<void()> click, const std::string &text, double sx, double sy,
                       const std::string &texture, tPos usualState, tPos selectedState, tPos pressedState,
                       const std::string& font, sf::Color textColor, const std::string& clickSound) {
    _buttons.push_back(Button{x, y, w, h, std::move(click), text, sx, sy, texture, usualState, selectedState, pressedState, font, textColor, clickSound});
    _buttons.back().init();
}

void Window::update() {

    _screen->setTitle(_name);
    _screen->drawSprite(_back);

    Vec2D mousePos = _mouse->getMousePosition();
    Vec2D dMousePos = mousePos - *_prevMousePosition;
    _back.setPosition(_back.getPosition() - sf::Vector2f((float)(dMousePos.x() / 30), (float)(dMousePos.y() / 30)));
    bool isPressed = _mouse->isButtonTapped(sf::Mouse::Left);

    for(auto& button : _buttons) {
        if( mousePos.x() > button.x() - button.w() * button.sx() / 2 && mousePos.y() > button.y() - button.h() * button.sy() / 2 &&
            mousePos.x() < button.x() + button.w() * button.sx() / 2 && mousePos.y() < button.y() + button.h() * button.sy() / 2) {
            button.select();
            if(isPressed)
                button.press();
        } else {
            button.unSelect();
        }

        if(_screen->isOpen()) {
            _screen->drawSprite(button.sprite());
            _screen->drawText(button.text());
        }
    }

    _prevMousePosition = std::make_unique<Vec2D>(mousePos);
}

void Window::setBackgroundTexture(const std::string &texture, double sx, double sy, int w, int h) {
    _backTexture = texture;
    std::shared_ptr<sf::Texture> t = ResourceManager::loadTexture(_backTexture);
    t->setRepeated(true);
    _back = sf::Sprite(*t, sf::IntRect(0, 0, (int)(w + w / 30.0), (int)(h + h / 30.0)));
    _back.scale((float)sx, (float)sy);
    _back.setPosition(sf::Vector2f(-w / 30.0f, -h / 30.0f));
}
