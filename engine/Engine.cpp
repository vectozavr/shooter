//
// Created by Иван Ильин on 14.01.2021.
//

#include "Engine.h"
#include "utils/Time.h"
#include <iostream>
#include "ResourceManager.h"
#include "physics/Solver.h"
#include "animation/Timeline.h"

Engine::Engine() {
    screen = std::make_shared<Screen>();
    keyboard = std::make_shared<Keyboard>();
    mouse = std::make_shared<Mouse>();

    world = std::make_shared<World>();
    camera = std::make_shared<Camera>();
}

void Engine::create(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background, sf::Uint32 style) {
    _name = name;

    screen->open(screenWidth, screenHeight, name, verticalSync, background, style);
    screen->attachMouse(mouse);

    Log::log("Engine::create(): started engine (" + std::to_string(screenWidth) + " x " + std::to_string(screenHeight) + ") with title '" + name + "'.");
    Time::update();

    start();
    camera->init(screenWidth, screenHeight);
    mouse->setMouseInCenter();

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

            Timeline::update();

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
    Log::log("Engine::exit(): exit engine (" + std::to_string(screen->width()) + " x " + std::to_string(screen->height()) + ") with title '" +
                     screen->title() + "'.");
}

void Engine::printDebugText() const {
    if (_debugText) {
        screen->debugText(_name + "\n\n X: " +
                          std::to_string((camera->position().x())) + "\n Y: " +
                          std::to_string((camera->position().y())) + "\n Z: " +
                          std::to_string((camera->position().z())) + "\n\n" +
                          std::to_string(screen->width()) + "x" +
                          std::to_string(screen->height()) + "\n" +
                          std::to_string(Time::fps()) +
                          " fps \n" + std::to_string((int) _triPerSec) + " _tris/s");
    }
}
