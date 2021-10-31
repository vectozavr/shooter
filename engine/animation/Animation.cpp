//
// Created by Иван Ильин on 27.01.2021.
//

#include "Animation.h"

Animation::Animation(double duration, Animation::LoopOut looped, Animation::InterpolationType intType, bool waitFor)
        : _duration(duration), _looped(looped), _intType(intType), _waitFor(waitFor) {
}

bool Animation::updateState() {
    if (_finished || std::abs(_duration) < Consts::EPS) {
        _finished = true;
        return false;
    }

    // linear normalized time:
    _dtime = Time::deltaTime() / _duration;
    _time += _dtime;

    if (_looped == LoopOut::Continue && _time > 0.5) {
        _time = 0.5;
    }

    switch (_intType) {
        case InterpolationType::Bezier:
            _progress = Interpolation::Bezier(Consts::BEZIER[0], Consts::BEZIER[1], _time);
            _dprogress = Interpolation::dBezier(Consts::BEZIER[0], Consts::BEZIER[1], _time, _dtime);
            break;
        case InterpolationType::Bouncing:
            _progress = Interpolation::Bouncing(_time);
            _dprogress = Interpolation::dBouncing(_time, _dtime);
            break;
        case InterpolationType::Linear:
            _progress = Interpolation::Linear(_time);
            _dprogress = Interpolation::dLinear(_time, _dtime);
            break;
        case InterpolationType::Cos:
            _progress = Interpolation::Cos(_time);
            _dprogress = Interpolation::dCos(_time, _dtime);
            break;
        default:
            throw std::logic_error{
                    "Animation::updateState: unknown interpolation type " + std::to_string(static_cast<int>(_intType))};
    }

    update();

    return (_time < 1) || _looped == LoopOut::Cycle;
}
