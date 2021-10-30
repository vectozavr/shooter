//
// Created by Иван Ильин on 11.01.2021.
//

#include "Time.h"
#include "../Consts.h"
#include "Log.h"

using namespace std::chrono;

Time* Time::_instance = nullptr;
bool Time::_validInstance = false;

void Time::init() {
    _instance = new Time();
    _validInstance = true;

    Log::log("Time::init(): time was initialized");
}

double Time::time() {
    if(!_validInstance) {
        return 0;
    }

    return _instance->_time;
}

double Time::deltaTime() {
    if(!_validInstance) {
        return 0;
    }

    return _instance->_deltaTime;
}

void Time::update() {
    if(!_validInstance) {
        return;
    }

    high_resolution_clock::time_point t = high_resolution_clock::now();

    _instance->_deltaTime = duration<double>(t - _instance->_last).count();
    _instance->_time = duration<double>(t - _instance->_start).count();
    // in case when fps < 10 it is useful to decrease _deltaTime (to avoid collision problems)
    if(_instance->_deltaTime > Consts::LARGEST_TIME_STEP)
        _instance->_deltaTime = Consts::LARGEST_TIME_STEP;

    _instance->_last = t;

    if(_instance->_deltaTime > 10000)
        return;

    _instance->_fpsCounter++;
    if (t - _instance->_fpsStart > _instance->_fpsCountTime) {
        _instance->_lastFps = _instance->_fpsCounter / duration<double>(t - _instance->_fpsStart).count();
        _instance->_fpsCounter = 0;
        _instance->_fpsStart = t;
    }
}

int Time::fps() {
    if(!_validInstance) {
        return 0;
    }
    // Cast is faster than floor and has the same behavior for positive numbers
    return static_cast<int>(_instance->_lastFps);
}

void Time::free() {
    _validInstance = false;
    delete _instance;

    Log::log("Time::free(): pointer to 'Time' was freed");
}
