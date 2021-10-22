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
    bool _useOpenGL = Consts::USE_OPEN_GL;

    void printDebugText() const;
protected:
    std::shared_ptr<Screen> screen;
    std::shared_ptr<Keyboard> keyboard;
    std::shared_ptr<Mouse> mouse;

    std::shared_ptr<World> world;
    std::shared_ptr<Camera> camera;

    virtual void start() {};
    virtual void update() {};

    void setDebugText(bool value) { _debugText = value; }
    void setUpdateWorld(bool value) { _updateWorld = value; }
    void setGlEnable(bool value) { _useOpenGL = value; }

    virtual void gui(){}
public:
    Engine();

    virtual ~Engine() = default;

    void create(int screenWidth = Consts::STANDARD_SCREEN_WIDTH, int screenHeight = Consts::STANDARD_SCREEN_HEIGHT, const std::string& name = Consts::PROJECT_NAME, bool verticalSync = true, sf::Color background = Consts::BACKGROUND_COLOR, sf::Uint32 style = sf::Style::Default);
    void exit();
};


#endif //INC_3DZAVR_TDZAVR_H
