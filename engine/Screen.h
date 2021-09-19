//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_SCREEN_H
#define ENGINE_SCREEN_H


#include <string>
#include "Triangle.h"
#include <SFML/Graphics.hpp>
#include <map>
#include "utils/Time.h"
#include "Mouse.h"

class Screen {
private:
    int _w = 1920;
    int _h = 1080;

    std::string _title;

    sf::Color _background;

    std::string _font = "../engine/fonts/Roboto-Thin.ttf";

    std::shared_ptr<sf::RenderWindow> _window;
public:
    void open(int screenWidth = 1920, int screenHeight = 1080, const std::string& name = "engine", bool verticalSync = true, sf::Color background = sf::Color(255, 255, 255), sf::Uint32 style = sf::Style::Default);

    void display();
    void clear();
    bool hasFocus() const { return _window->hasFocus(); }

    void drawTriangle(const Triangle& triangle);
    void drawTetragon(const Point4D& p1, const Point4D& p2, const Point4D& p3, const Point4D& p4, sf::Color color);
    void drawText(const std::string& string, const Point4D& position, int size, sf::Color color);
    void drawText(const sf::Text& text);
    void drawSprite(const sf::Sprite& sprite);

    void setTitle(const std::string& title);
    [[nodiscard]] std::string title() const { return _title; };

    bool isOpen();

    [[nodiscard]] int width() const {return _window->getSize().x;}
    [[nodiscard]] int height() const {return _window->getSize().y;}

    void close();

    void debugText(const std::string& text);

    void attachMouse(std::shared_ptr<Mouse> mouse);
};


#endif //INC_3DZAVR_SCREEN_H
