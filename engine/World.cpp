//
// Created by Иван Ильин on 13.01.2021.
//

#include <fstream>
#include <sstream>
#include "World.h"
#include "utils/Log.h"
#include "Plane.h"

using namespace std;

void World::addMesh(std::shared_ptr<Mesh> mesh, const string &name) {
    _objects.emplace(name, mesh);
    Log::log("World::addMesh(): inserted mesh '" + name + "' with " + std::to_string(_objects[name]->triangles().size()) + " tris.");

}

void World::loadObj(const string &name, const string &filename,const std::string &materials, const Point4D& scale) {
    _objects.emplace(name, std::make_shared<Mesh>(filename, materials, scale));
    Log::log("World::loadObj(): inserted mesh from " + filename + " with name '" + name + "' with " + std::to_string(_objects[name]->triangles().size()) + " tris.");
}

void World::removeMesh(const string &name) {
    _objToRemove.push_back(name);
}

std::shared_ptr<Mesh> World::operator[](const string &name) {
    if(_objects.count(name) == 0)
        Log::log("World::operator[]: mesh '" + name + "' does not exist.");
    return _objects.find(name)->second;
}

std::pair<Point4D, string> World::rayCast(const Point4D& from, const Point4D& to) {

    std::pair<Point4D, string> result{Point4D{0, 0,0, -1}, ""};
    double minDistance = 10000;

    for(auto& object : _objects) {
        if((object.first.find("im") != std::string::npos) || (object.first.find("point") != std::string::npos) || (object.first.find("nr") != std::string::npos))
            continue;

        for(auto& tri : object.second->triangles()) {
            Triangle tri_translated(tri[0] + object.second->position(), tri[1] + object.second->position(), tri[2] + object.second->position(), 0);

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

void World::loadMap(const string &filename, const string &name, const Point4D &scale, const string &materials) {
    auto objs = Mesh::LoadObjects(filename, materials, scale);
    for(int i = 0; i < objs.size(); i++) {
        string meshName = name + "_" + to_string(i);
        addMesh(objs[i], meshName);
    }
}

void World::garbageCollector() {
    for(auto& obj : _objToRemove) {
        if(_objects.erase(obj) > 0)
            Log::log("World::garbageCollector(): removed mesh '" + obj + "'");
        else
            Log::log("World::garbageCollector(): cannot remove mesh '" + obj + "': mesh does not exist.");
    }
    _objToRemove.clear();
}

void World::removeMeshInstantly(const string &name) {
    if(_objects.erase(name) > 0)
        Log::log("World::removeMeshInstantly(): removed mesh '" + name + "'");
    else
        Log::log("World::removeMeshInstantly(): cannot remove mesh '" + name + "': mesh does not exist.");
}
