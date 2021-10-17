//
// Created by Иван Ильин on 26.03.2021.
//

#include "Button.h"

#include <utility>
#include "../ResourceManager.h"

void Button::select()
{
    if (!_selected && !_pressed)
    {
        _button.setTextureRect(sf::IntRect(_selectedState.tx, _selectedState.ty, _w, _h));
        _selected = true;
    }
}

void Button::unSelect()
{
    if (_selected && !_pressed)
    {
        _button.setTextureRect(sf::IntRect(_usualState.tx, _usualState.ty, _w, _h));
        _selected = false;
    }
}

void Button::press()
{
    if (!_pressed)
    {
        _button.setTextureRect(sf::IntRect(_pressedState.tx, _pressedState.ty, _w, _h));
        if(_checkBox)
            _pressed = true;
        //_clickSound.play();
        _click();
    }
    else
    {
        _button.setTextureRect(sf::IntRect(_usualState.tx, _usualState.ty, _w, _h));
        if(_checkBox)
            _pressed = false;
    }
}

void Button::init() {
    _button.setTexture(*ResourceManager::loadTexture(_texture));
    _button.setTextureRect(sf::IntRect(_usualState.tx, _usualState.ty, _w, _h));
    _button.scale((float)_sx, (float)_sy);
    _button.setPosition((float)(_x - _w * _sx / 2), (float)(_y - _h * _sy / 2));

    _text.setFont(*ResourceManager::loadFont(_font));
    _text.setString(_textString);
    _text.setCharacterSize((unsigned int)(_h * _sy / 2));
    _text.setFillColor(_textColor);
    _text.setPosition((float)(_x - _text.getLocalBounds().width / 2), (float)(_y - _h * _sy / 2 + _text.getLocalBounds().height / 4));

    //_clickSound.setBuffer(*ResourceManager::loadSoundBuffer(_clickSoundName));
    //_clickSound.setVolume(15);
}

Button::Button(int x, int y, int width, int height, std::function<void()> click, std::string text, double sx,
               double sy, std::string texture, tPos usualState, tPos selectedState, tPos pressedState,
               std::string font, sf::Color textColor) : _x(x), _y(y), _w(width), _h(height), _click(std::move(click)),
               _textString(std::move(text)), _sx(sx), _sy(sy), _texture(std::move(texture)), _usualState(usualState), _selectedState(selectedState), _pressedState(pressedState),
               _font(std::move(font)), _textColor(textColor) {

}
