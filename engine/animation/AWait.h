//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_AWAIT_H
#define ENGINE_AWAIT_H

#include "Animatable.h"
#include "Animation.h"

class AWait : public Animation {
public:
    explicit AWait(double duration = 1) {
        _duration = duration;
        _intType = linear;
        _waitFor = true;
    }

    bool update() override {
        return updateState();
    }
};

#endif //INC_3DZAVR_AWAIT_H
