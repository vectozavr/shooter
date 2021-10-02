//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef ENGINE_ACOLOR_H
#define ENGINE_ACOLOR_H

#include "Animatable.h"
#include "Animation.h"
#include "Mesh.h"

class AColor : public Animation {
private:
    std::shared_ptr<Mesh> _mesh;

    sf::Color newColor;
    sf::Color startColor;

public:
    AColor(std::shared_ptr<Mesh> mesh, const sf::Color &color, double duration = 1, LoopOut looped = LoopOut::None, InterpolationType interpolationType = InterpolationType::linear) {
        _mesh = mesh;
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;
        _waitFor = true;

        newColor = color;
    }

    bool update() override {
        if(!_started)
            startColor = _mesh->color();

        Point4D start(startColor.r, startColor.g, startColor.b, startColor.a);
        Point4D end(newColor.r, newColor.g, newColor.b, newColor.a);
        Point4D mid = start + (end - start)*_p;

        _mesh->setColor(sf::Color(static_cast<sf::Uint8>(mid.x()), static_cast<sf::Uint8>(mid.y()), static_cast<sf::Uint8>(mid.z()), static_cast<sf::Uint8>(mid.w())));

        return updateState();
    }
};

#endif //SHOOTER_3DZAVR_ACOLOR_H
