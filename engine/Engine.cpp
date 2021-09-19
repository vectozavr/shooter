//
// Created by Иван Ильин on 14.01.2021.
//

#include "Engine.h"
#include "utils/Time.h"
#include <iostream>
#include "ResourceManager.h"
#include "physics/Solver.h"

Engine::Engine() {
    screen = std::make_shared<Screen>();
    world = std::make_shared<World>();
    camera = std::make_shared<Camera>();
}

void Engine::create(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background, sf::Uint32 style) {
    _name = name;

    screen->open(screenWidth, screenHeight, name, verticalSync, background, style);

    Log::log("Engine::create(): started engine (" + std::to_string(screenWidth) + " x " + std::to_string(screenHeight) + ") with name '" + name + "'.");
    Time::update();

    start();
    camera->init(screenWidth, screenHeight);
    screen->getMouseDisplacement(); // We do it to set mouse position in the center (see how getMouseDisplacement() works)

    while (screen->isOpen()) {
        screen->clear();

        Time::update();
        update();

        world->garbageCollector();
        /* Project all mesh
         * Here we project all _tris for each body from world._objects.
         * When we call camera.project(m.second),
         */

        // sometimes we dont need to update physics world
        // (for example in menu or while pause)
        // hence we can set '_updateWorld' equal to false in setUpdateWorld(bool):
        if(_updateWorld) {

            camera->a_update();
            camera->clear();

            world->update();
            world->projectObjectsInCamera(camera);

            // draw projected body
            for (auto &t : camera->sorted())
                screen->drawTriangle(t);

            _triPerSec = camera->buffSize() * Time::fps();

            printDebugText();
            gui();
        }

        screen->display();
    }
    exit();
}

void Engine::exit() {
    if(screen->isOpen()) {
        screen->close();
    }
    ResourceManager::unloadAllResources();
    Log::log("Engine::exit(): exit engine (" + std::to_string(screen->width()) + " x " + std::to_string(screen->height()) + ") with name '" + screen->name() + "'.");
}

void Engine::printDebugText() const {
    if (_debugText) {
        screen->debugText(_name + "\n\n X: " +
                          std::to_string((camera->eye().x())) + "\n Y: " +
                          std::to_string((camera->eye().y())) + "\n Z: " +
                          std::to_string((camera->eye().z())) + "\n\n" +
                          std::to_string(screen->width()) + "x" +
                          std::to_string(screen->height()) + "\n" +
                          std::to_string(Time::fps()) +
                          " fps \n" + std::to_string((int) _triPerSec) + " _tris/s");
    }
}
