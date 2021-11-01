//
// Created by Иван Ильин on 26.01.2021.
//

#ifndef ENGINE_ANIMATION_H
#define ENGINE_ANIMATION_H

#include "../utils/Time.h"
#include "../Triangle.h"
#include "Interpolation.h"
#include "../Vec2D.h"

class Animation {
public:
    enum class InterpolationType {
        Linear,
        Cos,
        Bezier,
        Bouncing
    };
    enum class LoopOut {
        None,
        Continue
    };
private:
    // normalized time (from 0 to 1)
    double _time = 0;
    double _dtime = 0;

    bool _finished = false;

    double _progress = 0;
    double _dprogress = 0;

    // If '_waitFor' == true then we need to finish all animation before starting this one. (for example for a_wait() or a_scale())
    const bool _waitFor = false;
    const double _duration = 0;
    const LoopOut _looped = LoopOut::None;
    const InterpolationType _intType = InterpolationType::Bezier;

    // You should override this method for your particular animation
    virtual void update() = 0;

public:
    Animation(double duration, LoopOut looped, InterpolationType intType, bool _waitFor = false);

    virtual ~Animation() = default;

    [[nodiscard]] bool waitFor() const { return _waitFor; }

    bool updateState();

    [[nodiscard]] double progress() const { return _progress; }

    [[nodiscard]] double dprogress() const { return _dprogress; }

    void stop() { _finished = true; }

    [[nodiscard]] bool isFinished() const { return _finished; }
};

#endif //INC_3DZAVR_ANIMATION_H
