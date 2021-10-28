//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <vector>
#include "Plane.h"
#include "Mesh.h"
#include <SFML/OpenGL.hpp>

class Camera final : public Object{
private:
    Matrix4x4 _S; // screen space matrix
    Matrix4x4 _P; // projections matrix
    Matrix4x4 _V; // camera matrix

    // To accelerate calculations we can use precalculated matrix that does not chance
    Matrix4x4 _SP; // screen-space-projections matrix

    std::vector<std::shared_ptr<Triangle>> _triangles{};
    std::vector<Plane> _clipPlanes{};

    bool _ready = false;
    double _aspect = 0;

public:
    Camera() : Object(ObjectNameTag("Camera")) {};
    Camera(const Camera& camera) = delete;

    void init(int width, int height, double fov = 110.0, double ZNear = 0.1, double ZFar = 5000.0);

    std::vector<std::shared_ptr<Triangle>> project(std::shared_ptr<Mesh> mesh);

    void clear();

    [[nodiscard]] int buffSize() const { return _triangles.size(); }
    std::vector<std::shared_ptr<Triangle>> sorted();

    // OpenGL function
    [[nodiscard]] GLfloat* view() const;
};


#endif //INC_3DZAVR_CAMERA_H
