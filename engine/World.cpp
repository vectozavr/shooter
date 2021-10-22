//
// Created by Иван Ильин on 13.01.2021.
//

#include "World.h"
#include "utils/Log.h"
#include "Plane.h"
#include "ResourceManager.h"
#include <sstream>
#include <cmath>

using namespace std;

void World::addBody(std::shared_ptr<RigidBody> body, const ObjectNameTag& tag) {
    _objects.emplace(tag, body);
    Log::log("World::addBody(): inserted body '" + tag.str() + "' with " + std::to_string(_objects[tag]->triangles().size()) + " tris.");
}

void World::loadBody(const ObjectNameTag& tag, const string &filename, const Vec3D& scale) {
    _objects.emplace(tag, std::make_shared<RigidBody>(Mesh(filename, scale)));
    Log::log("World::loadBody(): inserted body from " + filename + " with title '" + tag.str() + "' with " + std::to_string(_objects[tag]->triangles().size()) + " tris.");
}

IntersectionInformation World::rayCast(const Vec3D& from, const Vec3D& to, const std::string& skipTags) {

    // make vector of tags, that we are going to escape
    vector <string> tagsToSkip;
    stringstream s(skipTags);
    std::string t;
    while (s >> t) tagsToSkip.push_back(t);

    bool intersected = false;
    std::unique_ptr<Vec3D> point = std::make_unique<Vec3D>();
    std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>();
    std::string bodyName;
    double minDistance = Consts::RAY_CAST_MAX_DISTANCE;
    std::shared_ptr<RigidBody> intersectedBody = nullptr;

    for(auto& [name, body]  : _objects) {

        //for (auto& escapeTag : tagsToSkip)
        //    if(name.str().find(escapeTag) != std::string::npos)
        //        continue;

        if(name.str().find("Player") != std::string::npos || name.str().find("weapon") != std::string::npos)
            continue;

        for(auto& tri : body->triangles()) {
            Triangle tri_translated(tri[0] + body->position().makePoint4D(), tri[1] + body->position().makePoint4D(), tri[2] + body->position().makePoint4D());

            Plane plane(tri_translated);
            auto intersection = plane.intersection(from, to);
            double distance = (intersection.first - from).sqrAbs();
            if(intersection.second > 0 && distance < minDistance && tri_translated.isPointInside(intersection.first)) {
                minDistance = distance;
                point = std::make_unique<Vec3D>(intersection.first);
                triangle = std::make_unique<Triangle>(tri_translated);
                bodyName = name.str();
                intersected = true;
                intersectedBody = body;
            }
        }
    }
    return IntersectionInformation{*point, sqrt(minDistance), *triangle, ObjectNameTag(bodyName), intersectedBody, intersected};
}

void World::loadMap(const std::string& filename, const Vec3D& scale) {
    auto objs = ResourceManager::loadObjects(filename);
    for(unsigned i = 0; i < objs.size(); i++) {
        ObjectNameTag meshName = ObjectNameTag("map_" + to_string(i));
        addBody(std::make_shared<RigidBody>(*objs[i]), meshName);
        body(meshName)->scale(scale);
    }

    auto it = _objects.begin();
}

void World::removeBody(const ObjectNameTag& tag) {
    if(_objects.erase(tag) > 0)
        Log::log("World::removeBody(): removed body '" + tag.str() + "'");
    else
        Log::log("World::removeBody(): cannot remove body '" + tag.str() + "': body does not exist.");
}

void World::checkCollision(const ObjectNameTag& tag) {
    if (_objects[tag]->isCollision()) {

        _objects[tag]->setInCollision(false);

        for (auto it = _objects.begin(); it != _objects.end();) {

            auto obj = it->second;
            ObjectNameTag name = it->first;
            it++;

            if(name != tag) {
                std::pair<bool, Simplex> gjk = _objects[tag]->checkGJKCollision(obj);
                if (gjk.first) {
                    if (obj->isCollider()) {
                        CollisionPoint epa = _objects[tag]->EPA(gjk.second, obj);
                        _objects[tag]->solveCollision(epa);
                    }
                    if (_objects[tag]->collisionCallBack() != nullptr)
                        _objects[tag]->collisionCallBack()(name, obj);
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

std::shared_ptr<RigidBody> World::body(const ObjectNameTag& tag) {
    if(_objects.count(tag) == 0)
        return nullptr;
    return _objects.find(tag)->second;
}
