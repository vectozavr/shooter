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

class Screen {
private:
    int _w = 1920;
    int _h = 1080;

    std::string _name;

    sf::Color _background;

    std::map<sf::Keyboard::Key, double> _tappedKeys;
    std::map<sf::Mouse::Button, double> _tappedButtons;

    std::string _font = "../engine/fonts/Roboto-Thin.ttf";

public:
    sf::RenderWindow window;

    void open(int screenWidth = 1920, int screenHeight = 1080, const std::string& name = "engine", bool verticalSync = true, sf::Color background = sf::Color(255, 255, 255), sf::Uint32 style = sf::Style::Default);

    void display();
    void clear();

    void drawTriangle(const Triangle& triangle);

    void setName(const std::string& title);
    std::string name() const { return _name; };

    bool isOpen();

    int width() const {return window.getSize().x;}
    int height() const {return window.getSize().y;}

    void close();

    static bool isKeyPressed(sf::Keyboard::Key key); // returns true if this key is pressed
    bool isKeyTapped(sf::Keyboard::Key key); // returns true if this key is tapped and 1/5 sec passed (button bouncing problem solved)

    static bool isButtonPressed(sf::Mouse::Button button); // returns true if this button is pressed
    bool isButtonTapped(sf::Mouse::Button button); // returns true if this button is tapped and 1/5 sec passed (button bouncing problem solved)

    Point4D getMousePosition() const;
    Point4D getMouseDisplacement() const;
    void setMouseInCenter() const;
    void setMouseCursorVisible(bool visible);

    void debugText(const std::string& text);
};


#endif //INC_3DZAVR_SCREEN_H
