//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef ENGINE_ACOLOR_H
#define ENGINE_ACOLOR_H

#include "Animatable.h"
#include "Animation.h"

class AColor : public Animation {
private:
    sf::Color newColor;
    sf::Color startColor;

public:
    AColor(const sf::Color &color, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;
        _waitFor = true;

        newColor = color;
    }

    bool update(Animatable& obj) override {
        if(!_started)
            startColor = obj.color();

        Point4D start(startColor.r, startColor.g, startColor.b, startColor.a);
        Point4D end(newColor.r, newColor.g, newColor.b, newColor.a);
        Point4D mid = start + (end - start)*_p;

        obj.setColor(sf::Color(static_cast<sf::Uint8>(mid.x()), static_cast<sf::Uint8>(mid.y()), static_cast<sf::Uint8>(mid.z()), static_cast<sf::Uint8>(mid.w())));

        return updateState();
    }
};

#endif //SHOOTER_3DZAVR_ACOLOR_H
