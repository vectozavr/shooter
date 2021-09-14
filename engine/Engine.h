//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include "Screen.h"
#include "World.h"
#include "Camera.h"
#include "utils/Log.h"

class Engine {
protected:
    std::shared_ptr<Screen> screen;
    std::shared_ptr<World> world;
    std::shared_ptr<Camera> camera;

    double triPerSec = 0;

    bool b_debugText = true;
    bool b_updateWorld = true;
public:
    Engine();

    virtual ~Engine() = default;

    void create(int screenWidth = 1920, int screenHeight = 1080, const std::string& name = "engine", bool verticalSync = true, sf::Color background = sf::Color(255, 255, 255), sf::Uint32 style = sf::Style::Default);

    virtual void start() {};
    virtual void update(double elapsedTime) {};
    void exit();
    void debugText(bool value) { b_debugText = value; }
    void setUpdateWorld(bool value) { b_updateWorld = value; }

    virtual void gui(){}
};


#endif //INC_3DZAVR_TDZAVR_H
