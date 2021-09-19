//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_WORLD_H
#define ENGINE_WORLD_H

#include <map>
#include "Camera.h"
#include "physics/RigidBody.h"

class World {
private:
    std::map<std::string, std::shared_ptr<RigidBody>> _objects;
    std::vector<std::string> _objToRemove;
public:
    World() = default;

    void checkCollision(const std::string& body);
    void update();
    void projectObjectsInCamera(std::shared_ptr<Camera> camera);

    void addBody(std::shared_ptr<RigidBody> mesh, const std::string& name = "");
    std::shared_ptr<RigidBody> body(const std::string& name);
    void removeBody(const std::string& name);
    void removeBodyInstantly(const std::string& name);
    void garbageCollector();
    void loadBody(const std::string &name, const std::string &filename, const std::string &materials = "", const Point4D& scale = Point4D{1, 1, 1});

    // rayCast returns pair of Point4D and std::string:
    // 1) Point4D is point of collision (the last coordinate is -1 if there are no collisions)
    // 2) std::string - name of the object
    std::pair<Point4D, std::string> rayCast(const Point4D& from, const Point4D& to);

    void loadMap(const std::string& filename, const std::string& name = "", const Point4D& scale = Point4D{1, 1, 1}, const std::string &materials = "../maps/materials.txt");
};


#endif //INC_3DZAVR_WORLD_H
