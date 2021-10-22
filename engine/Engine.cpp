//
// Created by Иван Ильин on 14.01.2021.
//

#include "Engine.h"
#include "utils/Time.h"
#include <iostream>
#include "ResourceManager.h"
#include "animation/Timeline.h"
#include "SoundController.h"

Engine::Engine() {
    Time::init();
    Timeline::init();
    ResourceManager::init();
    SoundController::init();

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

    Log::log("Engine::create(): started engine (" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) + ") with title '" + name + "'.");
    Time::update();

    start();
    camera->init(screenWidth, screenHeight);
    mouse->setMouseInCenter();

    while (screen->isOpen()) {
        screen->clear();

        Time::update();
        update();

        // sometimes we dont need to update physics world
        // (for example in menu or while pause)
        // hence we can set '_updateWorld' equal to false in setUpdateWorld(bool):
        if(_updateWorld) {

            Timeline::update();

            world->update();

            if(_useOpenGL) {
                GLfloat* view = camera->view();
                for(auto & it : *world) {
                    if (it.second->isVisible()) {
                        GLfloat* geometry = Screen::glMeshToGLfloatArray(it.second, camera->position());
                        screen->glDrawMesh(geometry, view, 3 * it.second->triangles().size());
                        free(geometry);
                    }
                }
                free(view);
            } else {
                // clear triangles from previous frame
                camera->clear();
                // project triangles to the camera plane
                for(auto & it : *world)
                    camera->project(it.second);
                // draw triangles on the screen
                for (auto &t : camera->sorted())
                    screen->drawTriangle(*t);

                _triPerSec = camera->buffSize() * Time::fps();
            }

            printDebugText();
            gui();
        }

        screen->display();
    }
}

void Engine::exit() {
    if(screen->isOpen()) {
        screen->close();
    }
    SoundController::free();
    ResourceManager::free();
    Timeline::free();
    Time::free();

    Log::log("Engine::exit(): exit engine (" + std::to_string(screen->width()) + "x" + std::to_string(screen->height()) + ") with title '" +
                     screen->title() + "'.");
}

void Engine::printDebugText() const {
    if (_debugText) {
        std::string str = _name + "\n\n X: " +
                          std::to_string((camera->position().x())) + "\n Y: " +
                          std::to_string((camera->position().y())) + "\n Z: " +
                          std::to_string((camera->position().z())) + "\n\n" +
                          std::to_string(screen->width()) + "x" +
                          std::to_string(screen->height()) + "\t" +
                          std::to_string(Time::fps()) + " fps";
        if(_useOpenGL) {
            str += "\n Using OpenGL acceleration";
        } else {
            str += "\n" + std::to_string((int) _triPerSec) + " tris/s";
        }
        screen->debugText(str);
    }
}
