//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_WORLD_H
#define ENGINE_WORLD_H

#include <map>
#include "Camera.h"
#include "physics/RigidBody.h"

class World final {
private:
    std::map<std::string, std::shared_ptr<RigidBody>> _objects;
public:
    World() = default;

    void checkCollision(const std::string& body);
    void update();
    void projectObjectsInCamera(std::shared_ptr<Camera> camera);

    void addBody(std::shared_ptr<RigidBody> mesh, const std::string& name = "");
    std::shared_ptr<RigidBody> body(const std::string& name);
    void removeBody(std::string name);
    void loadBody(const std::string &name, const std::string &filename, const std::string &materials = "", const Vec3D& scale = Vec3D{1, 1, 1});

    // rayCast returns pair of Point4D and std::string:
    // 1) Point4D is point of collision
    // 2) std::string - title of the object
    std::pair<Vec3D, std::string> rayCast(const Vec3D& from, const Vec3D& to);

    void loadMap(const std::string& filename, const std::string& materials, const std::string& name = "map", const Vec3D & scale = Vec3D{1, 1, 1});
};


#endif //INC_3DZAVR_WORLD_H
