//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef ENGINE_ACOLOR_H
#define ENGINE_ACOLOR_H

#include <utility>

#include "Animation.h"
#include "../Mesh.h"

class AColor final : public Animation {
private:
    const std::weak_ptr<Mesh> _mesh;

    sf::Color _startColor;
    const sf::Color _newColor;
    bool _started = false;

    void update() override {
        if(_mesh.expired()) {
            stop();
            return;
        }

        if(!_started) {
            _started = true;
            _startColor = _mesh.lock()->color();
        }

        Vec4D start(_startColor.r, _startColor.g, _startColor.b, _startColor.a);
        Vec4D end(_newColor.r, _newColor.g, _newColor.b, _newColor.a);
        Vec4D mid = start + (end - start) * progress();

        _mesh.lock()->setColor(sf::Color(static_cast<sf::Uint8>(mid.x()), static_cast<sf::Uint8>(mid.y()), static_cast<sf::Uint8>(mid.z()), static_cast<sf::Uint8>(mid.w())));
    }
public:
    AColor(std::weak_ptr<Mesh> mesh, const sf::Color &color, double duration = 1, LoopOut looped = LoopOut::None, InterpolationType interpolationType = InterpolationType::Linear) : Animation(duration, looped, interpolationType), _mesh(std::move(mesh)), _newColor(color) {
    }
};

#endif //SHOOTER_3DZAVR_ACOLOR_H
