//
// Created by Иван Ильин on 13.01.2021.
//

#include "World.h"
#include "utils/Log.h"
#include "Plane.h"
#include "physics/Solver.h"

using namespace std;

void World::addBody(std::shared_ptr<RigidBody> body, const string &name) {
    _objects.emplace(name, body);
    Log::log("World::addBody(): inserted body '" + name + "' with " + std::to_string(_objects[name]->triangles().size()) + " tris.");
}

void World::loadBody(const string &name, const string &filename, const std::string &materials, const Point4D& scale) {
    _objects.emplace(name, std::make_shared<RigidBody>(Mesh(filename, materials, scale)));
    Log::log("World::loadBody(): inserted body from " + filename + " with title '" + name + "' with " + std::to_string(_objects[name]->triangles().size()) + " tris.");
}

std::pair<Point4D, string> World::rayCast(const Point4D& from, const Point4D& to) {

    std::pair<Point4D, string> result{Point4D{0, 0,0, -1}, ""};
    double minDistance = 10000;

    for(auto& object : _objects) {
        if((object.first.find("Player") != std::string::npos) || (object.first.find("Bonus") != std::string::npos))
            continue;

        for(auto& tri : object.second->triangles()) {
            Triangle tri_translated(tri[0] + object.second->position(), tri[1] + object.second->position(), tri[2] + object.second->position());

            Plane plane(tri_translated);
            auto intersection = plane.intersection(from, to);
            double distance = (intersection.first - from).sqrAbs();
            if(intersection.second > 0 && distance < minDistance && tri_translated.isPointInside(intersection.first)) {
                minDistance = distance;
                result = {intersection.first, object.first};
            }
        }
    }
    return result;
}

void World::loadMap(const std::string& filename, const std::string& materials, const std::string& name, const Point4D& scale) {
    auto objs = Mesh::LoadObjects(filename, materials, scale);
    for(unsigned i = 0; i < objs.size(); i++) {
        string meshName = name + "_" + to_string(i);
        addBody(std::make_shared<RigidBody>(*objs[i]), meshName);
    }
}

void World::removeBody(string name) {
    if(_objects.erase(name) > 0)
        Log::log("World::removeBody(): removed body '" + name + "'");
    else
        Log::log("World::removeBody(): cannot remove body '" + name + "': body does not exist.");
}

void World::checkCollision(const std::string& body) {
    if (_objects[body]->isCollision()) {

        _objects[body]->setInCollision(false);

        for (auto it = _objects.begin(); it != _objects.end();) {

            auto obj = it->second;
            std::string name = it->first;
            it++;

            if(name != body) {
                std::pair<bool, Simplex> gjk = _objects[body]->checkGJKCollision(obj);
                if (gjk.first) {
                    if (obj->isCollider()) {
                        CollisionPoint epa = _objects[body]->EPA(gjk.second, obj);
                        Solver::solveCollision(_objects[body], obj, epa);
                    }
                    if (_objects[body]->collisionCallBack() != nullptr)
                        _objects[body]->collisionCallBack()(name, obj);
                }
            }
        }
    }
}

void World::update() {
    for (auto &m : _objects) {
        m.second->updatePhysicsState();
        checkCollision(m.first);
    }
}

void World::projectObjectsInCamera(std::shared_ptr<Camera> camera) {
    for (auto &m : _objects)
        camera->project(m.second);
}

std::shared_ptr<RigidBody> World::body(const string &name) {
    if(_objects.count(name) == 0)
        Log::log("World::body: mesh '" + name + "' does not exist.");
    return _objects.find(name)->second;
}
