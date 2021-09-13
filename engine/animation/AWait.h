//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_AWAIT_H
#define ENGINE_AWAIT_H

#include "Animatable.h"
#include "Animation.h"

class AWait : public Animation {
private:
    Point4D value;
public:
    explicit AWait(double duration) {
        _duration = duration;
        _intType = linear;
        _waitFor = true;
    }

    bool update(Animatable& obj) override {
        return updateState();
    }

    [[nodiscard]] int type() const override {
        return 1;
    }
};

#endif //INC_3DZAVR_AWAIT_H
