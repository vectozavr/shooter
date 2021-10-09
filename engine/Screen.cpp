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
    _title = name;
    _w = screenWidth;
    _h = screenHeight;
    _background = background;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    _window = std::make_shared<sf::RenderWindow>();
    _window->create(sf::VideoMode(_w, _h), name, style, settings);
    _window->setVerticalSyncEnabled(verticalSync);
}

void Screen::display() {
    sf::Event event{};
    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window->close();
        }
    }

    std::string title = _title + " (" + std::to_string(Time::fps()) + " fps)";
    _window->setTitle(title);

    _window->display();
}

void Screen::clear() {
    _window->clear(_background);
}

void Screen::drawTriangle(const Triangle& triangle)
{
    sf::Vertex tris[3] =
            {
                    sf::Vertex(sf::Vector2f((float)triangle[0].x(), (float)triangle[0].y()), triangle.color()),
                    sf::Vertex(sf::Vector2f((float)triangle[1].x(), (float)triangle[1].y()), triangle.color()),
                    sf::Vertex(sf::Vector2f((float)triangle[2].x(), (float)triangle[2].y()), triangle.color())
            };
    _window->draw(tris, 3, sf::Triangles);
}

void Screen::setTitle(const std::string& title) {
    _title = title;
}

bool Screen::isOpen() {
    return _window->isOpen();
}

void Screen::close() {
    _window->close();
}


void Screen::debugText(const std::string& text) {
    sf::Text t;

    t.setFont(*ResourceManager::loadFont(_font));
    t.setString(text);
    t.setCharacterSize(30);
    t.setFillColor(sf::Color::Black);
    t.setPosition(10, 10);

    _window->draw(t);
}

void Screen::drawTetragon(const Point4D &p1, const Point4D &p2, const Point4D &p3, const Point4D &p4, sf::Color color) {
    sf::ConvexShape polygon;
    polygon.setPointCount(4);
    polygon.setPoint(0, sf::Vector2f((float)p1.x(), (float)p1.y()));
    polygon.setPoint(1, sf::Vector2f((float)p2.x(), (float)p2.y()));
    polygon.setPoint(2, sf::Vector2f((float)p3.x(), (float)p3.y()));
    polygon.setPoint(3, sf::Vector2f((float)p4.x(), (float)p4.y()));
    polygon.setFillColor(color);
    _window->draw(polygon);
}

void Screen::drawText(const std::string& string, const Point4D &position, int size, sf::Color color) {
    sf::Text text;

    text.setFont(*ResourceManager::loadFont("engine/fonts/Roboto-Medium.ttf"));

    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setStyle(sf::Text::Italic);
    text.setPosition((float)position.x(), (float)position.y());

    text.setString(string);
    _window->draw(text);
}

void Screen::drawSprite(const sf::Sprite &sprite) {
    _window->draw(sprite);
}

void Screen::drawText(const sf::Text &text) {
    _window->draw(text);
}

void Screen::attachMouse(std::shared_ptr<Mouse> mouse) {
    mouse->setWindow(_window);
}
