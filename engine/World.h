//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_WORLD_H
#define ENGINE_WORLD_H

#include <map>
#include "Mesh.h"

class World {
private:
    std::map<std::string, std::shared_ptr<Mesh>> _objects;

    std::vector<std::string> _objToRemove;
public:
    World() = default;

    [[nodiscard]] std::shared_ptr<Mesh> operator[] (const std::string& name);

    [[nodiscard]] std::map<std::string, std::shared_ptr<Mesh>>& objects() { return _objects; }

    void addMesh(std::shared_ptr<Mesh> mesh, const std::string& name = "");
    void removeMesh(const std::string& name);
    void removeMeshInstantly(const std::string& name);
    void garbageCollector();
    void loadObj(const std::string &name, const std::string &filename,const std::string &materials = "", const Point4D& scale = Point4D{1, 1, 1});

    // rayCast returns pair of Point4D and std::string:
    // 1) Point4D is point of collision (the last coordinate is -1 if there are no collisions)
    // 2) std::string - name of the object
    std::pair<Point4D, std::string> rayCast(const Point4D& from, const Point4D& to);

    void loadMap(const std::string& filename, const std::string& name = "", const Point4D& scale = Point4D{1, 1, 1}, const std::string &materials = "../maps/materials.txt");
};


#endif //INC_3DZAVR_WORLD_H
