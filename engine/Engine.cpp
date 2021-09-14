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
    screen->open(screenWidth, screenHeight, name, verticalSync, background, style);

    Log::log("Engine::create(): started engine (" + std::to_string(screenWidth) + " x " + std::to_string(screenHeight) + ") with name '" + name + "'.");
    Time::update();

    start();
    camera->init(screenWidth, screenHeight);
    screen->getMouseDisplacement(); // We do it to set mouse position in the center (see how getMouseDisplacement() works)

    while (screen->isOpen()) {
        screen->clear();

        Time::update();
        screen->keyboardControl();
        update(Time::deltaTime());

        world->garbageCollector();
        /* Project all mesh
         * Here we project all tris for each mesh from world._objects.
         * When we call camera.project(m.second),
         */

        // sometimes we dont need to update physics world
        // (for example in menu or while pause)
        // hence we can set 'b_updateWorld' equal to false in setUpdateWorld(bool):
        if(b_updateWorld) {
            camera->record();
            for (auto &m : world->objects()) {
                m.second->a_update();
                camera->project(*m.second, screen->mode());

                m.second->updatePhysicsState();
                // isCollision detection:
                if (m.second->isCollision()) {
                    m.second->setInCollision(false);
                    m.second->setCollisionNormal(Point4D{0, 0, 0});
                    for (auto &obj : world->objects()) {
                        if(obj.first != m.first) {
                            std::pair<bool, Simplex> gjk = m.second->checkGJKCollision(obj.second);
                            if (gjk.first) {
                                if (obj.second->isCollider()) {
                                    CollisionPoint epa = m.second->EPA(gjk.second, obj.second);
                                    Solver::solveCollision(m.second, obj.second, epa);
                                }
                                if (m.second->collisionCallBack() != nullptr)
                                    m.second->collisionCallBack()(obj.first, obj.second);
                            }
                        }
                    }
                }

            }

            // draw projected mesh
            for (auto &t : camera->sorted())
                screen->triangle(t);

            camera->a_update();

            triPerSec = camera->buffSize() * Time::fps();

            if (b_debugText) {
                screen->debugText(name + "\n\n X: " +
                                 std::to_string((camera->eye().x())) + "\n Y: " +
                                 std::to_string((camera->eye().y())) + "\n Z: " +
                                 std::to_string((camera->eye().z())) + "\n\n" +
                                 std::to_string(screen->width()) + "x" +
                                 std::to_string(screen->height()) + "\n" +
                                 std::to_string(Time::fps()) +
                                 " fps \n" + std::to_string((int) triPerSec) + " tris/s");
            }
        }

        gui();
        screen->display();
    }
    exit();
}

void Engine::exit() {
    if(screen->isOpen()) {
        screen->close();
    }
    ResourceManager::unloadAllResources();
    Log::log("Engine::exit(): exit engine (" + std::to_string(screen->width()) + " x " + std::to_string(screen->height()) + ") with name '" + screen->title() + "'.");
}
