//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include "Screen.h"
#include "Keyboard.h"
#include "Mouse.h"

#include "World.h"
#include "Camera.h"
#include "utils/Log.h"

class Engine {
private:
    std::string _name;
    double _triPerSec = 0;
    bool _debugText = true;
    bool _updateWorld = true;

    void printDebugText() const;

protected:
    std::shared_ptr<Screen> screen;
    std::shared_ptr<Keyboard> keyboard;
    std::shared_ptr<Mouse> mouse;

    std::shared_ptr<World> world;
    std::shared_ptr<Camera> camera;

public:
    Engine();

    virtual ~Engine() = default;

    void create(int screenWidth = 1920, int screenHeight = 1080, const std::string& name = "engine", bool verticalSync = true, sf::Color background = sf::Color(255, 255, 255), sf::Uint32 style = sf::Style::Default);

    virtual void start() {};
    virtual void update() {};
    void exit();

    void setDebugText(bool value) { _debugText = value; }
    void setUpdateWorld(bool value) { _updateWorld = value; }

    virtual void gui(){}
};


#endif //INC_3DZAVR_TDZAVR_H
