//
// Created by Иван Ильин on 27.01.2021.
//

#include "Animation.h"

bool Animation::updateState() {
    if(!_started) {
        _started = true;
        return _duration != 0;
    }

    // linear normalized time:
    _dtime = Time::deltaTime()/_duration;
    _time += _dtime;

    if(_looped == LoopOut::Continue && _time > 0.5)
        _time = 0.5;

    switch (_intType) {
        case InterpolationType::bezier:
            _p = Interpolation::Bezier(*_bezier[0], *_bezier[1], _time);
            _dp = Interpolation::dBezier(*_bezier[0], *_bezier[1], _time, _dtime);
            break;
        case InterpolationType::bouncing:
            _p = Interpolation::Bouncing(_time);
            _dp = Interpolation::dBouncing(_time, _dtime);
            break;
        case InterpolationType::linear:
            _p = Interpolation::Linear(_time);
            _dp = Interpolation::dLinear(_time, _dtime);
            break;
        case InterpolationType::cos:
            _p = Interpolation::Cos(_time);
            _dp = Interpolation::dCos(_time, _dtime);
            break;
    }

    return (_time < 1) || _looped == LoopOut::Cycle;
}