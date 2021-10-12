//
// Created by Иван Ильин on 26.03.2021.
//

#ifndef ENGINE_BUTTON_H
#define ENGINE_BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>

struct tPos {
    int tx;
    int ty;
};

class Button final {
private:
    int _x{};
    int _y{};

    int _w{};
    int _h{};

    std::function<void()> _click;

    std::string _textString;

    double _sx{};
    double _sy{};

    std::string _texture;
    tPos _usualState{};
    tPos _selectedState{};
    tPos _pressedState{};

    std::string _font;
    sf::Color _textColor;

    std::string _clickSoundName;

    sf::Sprite _button;
    sf::Text _text;
    sf::Sound _clickSound;

    bool _selected = false;
    bool _pressed = false;
    bool _checkBox = false;

public:
    Button() = default;
    Button(int x, int y, int width, int height, std::function<void()> click, std::string  text, double sx, double sy, std::string  texture, tPos usualState, tPos selectedState, tPos pressedState, std::string  font, sf::Color textColor, std::string  clickSound);

    void select();
    void unSelect();
    void press();

    void init();

    [[nodiscard]] int x() const { return _x; }
    [[nodiscard]] int y() const { return _y; }
    [[nodiscard]] int w() const { return _w; }
    [[nodiscard]] int h() const { return _h; }
    [[nodiscard]] double sx() const { return _sx; }
    [[nodiscard]] double sy() const { return _sy; }
    [[nodiscard]] sf::Sprite const& sprite() const { return _button; }
    [[nodiscard]] sf::Text const& text() const { return _text; }
};


#endif //MINECRAFT_3DZAVR_BUTTON_H
