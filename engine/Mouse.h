//
// Created by Иван Ильин on 19.09.2021.
//

#ifndef SHOOTER_MOUSE_H
#define SHOOTER_MOUSE_H

#include <SFML/Graphics.hpp>
#include "utils/Point4D.h"

class Mouse {
private:
    std::shared_ptr<sf::RenderWindow> _window;

    std::map<sf::Mouse::Button, double> _tappedButtons;
public:
    Mouse() = default;

    void setWindow(std::shared_ptr<sf::RenderWindow> window);

    static bool isButtonPressed(sf::Mouse::Button button); // returns true if this _button is _pressed
    bool isButtonTapped(sf::Mouse::Button button); // returns true if this _button is tapped and 1/5 sec passed (_button bouncing problem solved)

    [[nodiscard]] Point4D getMousePosition() const;
    [[nodiscard]] Point4D getMouseDisplacement() const;
    void setMouseInCenter() const;
    void setMouseCursorVisible(bool visible);
};


#endif //SHOOTER_MOUSE_H
