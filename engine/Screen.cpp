//
// Created by Иван Ильин on 14.01.2021.
//

#include "Screen.h"
#include "utils/Time.h"
#include <utility>
#include "utils/Log.h"
#include "ResourceManager.h"
#include <cstdio>


void Screen::open(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background, sf::Uint32 style) {
    _name = name;
    _w = screenWidth;
    _h = screenHeight;
    _background = background;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window.create(sf::VideoMode(_w, _h), name, style, settings);
    window.setVerticalSyncEnabled(verticalSync);
}

void Screen::display() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    std::string title = _name + " (" + std::to_string(Time::fps()) + " fps)";
    window.setTitle(title);

    window.display();
}

void Screen::clear() {
    window.clear(_background);
}

void Screen::drawTriangle(const Triangle& triangle)
{
    sf::Vertex tris[3] =
            {
                    sf::Vertex(sf::Vector2f(triangle[0].x(), triangle[0].y()), triangle.color()),
                    sf::Vertex(sf::Vector2f(triangle[1].x(), triangle[1].y()), triangle.color()),
                    sf::Vertex(sf::Vector2f(triangle[2].x(), triangle[2].y()), triangle.color())
            };
    window.draw(tris, 3, sf::Triangles);
}

void Screen::setName(const std::string& title) {
    _name = title;
}

bool Screen::isOpen() {
    return window.isOpen();
}

void Screen::close() {
    window.close();
}

bool Screen::isKeyPressed(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

Point4D Screen::getMousePosition() const {
    sf::Vector2<int> pos = sf::Mouse::getPosition(window);
    return Point4D(pos.x, pos.y, 0, 0);
}

Point4D Screen::getMouseDisplacement() const {
    sf::Vector2<int> disp = sf::Mouse::getPosition(window) - sf::Vector2<int>(_w/2, _h/2);
    setMouseInCenter();
    return Point4D(disp.x, disp.y, 0, 0);
}

void Screen::setMouseInCenter() const {
    sf::Mouse::setPosition({ _w / 2, _h / 2 }, window);
}

void Screen::setMouseCursorVisible(bool visible) {
    window.setMouseCursorVisible(visible);
}

bool Screen::isKeyTapped(sf::Keyboard::Key key) {
    if (!Screen::isKeyPressed(key))
        return false;

    if(_tappedKeys.count(key) == 0) {
        _tappedKeys.emplace(key, Time::time());
        return true;
    } else if((Time::time() - _tappedKeys[key]) > 0.2) {
        _tappedKeys[key] = Time::time();
        return true;
    }
    return false;
}

bool Screen::isButtonPressed(sf::Mouse::Button button) {
    return sf::Mouse::isButtonPressed(button);
}

bool Screen::isButtonTapped(sf::Mouse::Button button) {
    if (!Screen::isButtonPressed(button))
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


void Screen::debugText(const std::string& text) {
    sf::Text t;

    t.setFont(*ResourceManager::loadFont(_font));
    t.setString(text);
    t.setCharacterSize(30);
    t.setFillColor(sf::Color::Black);
    t.setPosition(10, 10);

    window.draw(t);
}
