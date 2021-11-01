//
// Created by Иван Ильин on 01.11.2021.
//

#ifndef SHOOTER_AROTATELEFT_H
#define SHOOTER_AROTATELEFT_H

#include <utility>

#include "Animation.h"
#include "../Object.h"

class ARotateLeft final : public Animation {
private:
    const std::weak_ptr<Object> _object;
    const double _rotationValue;

    void update() override {
        if (_object.expired()) {
            stop();
            return;
        }

        _object.lock()->rotateLeft(-_object.lock()->angleLeftUpLookAt().x());
        _object.lock()->rotateLeft(_rotationValue*(progress()));
    }

public:
    ARotateLeft(std::weak_ptr<Object> object, double r, double duration = 1, LoopOut looped = LoopOut::None,
            InterpolationType interpolationType = InterpolationType::Bezier)
            : Animation(duration, looped, interpolationType), _object(object), _rotationValue(r) {}
};

#endif //SHOOTER_AROTATELEFT_H
