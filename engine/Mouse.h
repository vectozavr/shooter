//
// Created by Иван Ильин on 19.09.2021.
//

#ifndef SHOOTER_MOUSE_H
#define SHOOTER_MOUSE_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "Vec2D.h"

class Mouse final {
private:
    std::shared_ptr<sf::RenderWindow> _window;

    std::map<sf::Mouse::Button, double> _tappedButtons;
public:
    Mouse() = default;

    void setWindow(std::shared_ptr<sf::RenderWindow> window);

    static bool isButtonPressed(sf::Mouse::Button button); // returns true if this _button is _pressed
    bool isButtonTapped(sf::Mouse::Button button); // returns true if this _button is tapped and 1/5 sec passed (_button bouncing problem solved)

    [[nodiscard]] Vec2D getMousePosition() const;
    [[nodiscard]] Vec2D getMouseDisplacement() const;
    void setMouseInCenter() const;
    void setMouseCursorVisible(bool visible);
};


#endif //SHOOTER_MOUSE_H
