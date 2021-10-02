//
// Created by Иван Ильин on 06.04.2021.
//

#include <utility>

#include "Animation.h"

#ifndef ENGINE_AFUNCTION_H
#define ENGINE_AFUNCTION_H

class AFunction : public Animation {
private:
    int _calls = 0;
    int _allCalls = 1;
    std::function<void()> _callBack;

public:
    explicit AFunction(std::function<void()> function, int calls = 1, double duration = 1, LoopOut looped = LoopOut::None, InterpolationType interpolationType = InterpolationType::linear) {
        _callBack = std::move(function);
        _allCalls = calls;
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;
    }

    bool update() override {
        if(_allCalls != 0 && _p >= (double)(_calls+1) / (_allCalls+1)) {
            _calls++;
            _callBack();
        }
        return updateState();
    }
};

#endif //MINECRAFT_3DZAVR_AFUNCTION_H
