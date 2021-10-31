//
// Created by Иван Ильин on 11.01.2021.
//

#ifndef ENGINE_TIME_H
#define ENGINE_TIME_H

#include <chrono>

class Time final {
private:
    // High precision time
    std::chrono::high_resolution_clock::time_point _start = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point _last = _start;

    // FPS counter
    std::chrono::high_resolution_clock::time_point _fpsStart{};
    std::chrono::milliseconds _fpsCountTime = std::chrono::milliseconds(1000);
    int _fpsCounter = 0;
    double _lastFps = 0;

    // Compatibility
    double _time = 0;
    double _deltaTime = 0;

    static Time *_instance;
    static bool _validInstance;

    Time() = default;

public:
    Time(const Time &) = delete;

    Time &operator=(Time &) = delete;

    static int fps();

    static double time();

    static double deltaTime();

    static void update();

    static void init();

    static void free();
};

#endif //INC_3DZAVR_TIME_H
