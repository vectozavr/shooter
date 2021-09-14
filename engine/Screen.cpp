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
    this->name = name;
    w = screenWidth;
    h = screenHeight;
    this->background = background;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window.create(sf::VideoMode(w, h), name, style, settings);
    window.setVerticalSyncEnabled(verticalSync);
}

void Screen::display() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    std::string title = name + " (" + std::to_string(Time::fps()) + " fps)";
    window.setTitle(title);

    window.display();
}

void Screen::clear() {
    window.clear(background);
}

void Screen::triangle(const Triangle& triangle)
{
    if(vm == Frame || vm == Borders || vm == Xray || vm == Clipped || vm == Transparency || vm == Normals) {
        sf::Vertex lines[4] =
                {
                        sf::Vertex(sf::Vector2f(triangle[0].x(), triangle[0].y()), sf::Color(0, 0, 0, 255)),
                        sf::Vertex(sf::Vector2f(triangle[1].x(), triangle[1].y()), sf::Color(0, 0, 0, 255)),
                        sf::Vertex(sf::Vector2f(triangle[2].x(), triangle[2].y()), sf::Color(0, 0, 0, 255)),
                        sf::Vertex(sf::Vector2f(triangle[0].x(), triangle[0].y()), sf::Color(0, 0, 0, 255))
                };

        window.draw(lines, 4, sf::LineStrip);
    }
    if(vm == Frame || vm == Xray)
        return; // no texture when we turn on Frame or Xray mode

    sf::Vertex tris[3] =
            {
                    sf::Vertex(sf::Vector2f(triangle[0].x(), triangle[0].y()), triangle.color),
                    sf::Vertex(sf::Vector2f(triangle[1].x(), triangle[1].y()), triangle.color),
                    sf::Vertex(sf::Vector2f(triangle[2].x(), triangle[2].y()), triangle.color)
            };
    window.draw(tris, 3, sf::Triangles);
}

void Screen::title(const std::string& title)
{
    name = title;
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
    sf::Vector2<int> disp = sf::Mouse::getPosition(window) - sf::Vector2<int>(w/2, h/2);
    setMouseInCenter();
    return Point4D(disp.x, disp.y, 0, 0);
}

void Screen::setMouseInCenter() const {
    sf::Mouse::setPosition({ w / 2, h / 2 }, window);
}

void Screen::setMouseCursorVisible(bool visible) {
    window.setMouseCursorVisible(visible);
}

void Screen::keyboardControl() {
    // Check all input after this condition please
    if (!window.hasFocus())
        return;

    if(isKeyTapped(sf::Keyboard::Num1))
        setMode(ViewMode::Default);
    if(isKeyTapped(sf::Keyboard::Num2))
        setMode(ViewMode::Borders);
    if(isKeyTapped(sf::Keyboard::Num3))
        setMode(ViewMode::Transparency);
    if(isKeyTapped(sf::Keyboard::Num4))
        setMode(ViewMode::Frame);
    if(isKeyTapped(sf::Keyboard::Num5))
        setMode(ViewMode::Xray);
    if(isKeyTapped(sf::Keyboard::Num6))
        setMode(ViewMode::Clipped);
    if(isKeyTapped(sf::Keyboard::Num7))
        setMode(ViewMode::Normals);
}

bool Screen::isKeyTapped(sf::Keyboard::Key key) {
    if (!Screen::isKeyPressed(key))
        return false;

    if(tappedKeys.count(key) == 0) {
        tappedKeys.emplace(key, Time::time());
        return true;
    } else if((Time::time() - tappedKeys[key]) > 0.2) {
        tappedKeys[key] = Time::time();
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

    if(tappedButtons.count(button) == 0) {
        tappedButtons.emplace(button, Time::time());
        return true;
    } else if((Time::time() - tappedButtons[button]) > 0.2) {
        tappedButtons[button] = Time::time();
        return true;
    }
    return false;
}


void Screen::debugText(const std::string& text) {
    sf::Text t;

    t.setFont(*ResourceManager::loadFont(font));
    t.setString(text);
    t.setCharacterSize(30);
    t.setFillColor(sf::Color::Black);
    t.setPosition(10, 10);

    window.draw(t);
}
