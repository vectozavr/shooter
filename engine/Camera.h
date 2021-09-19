//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <vector>
#include "Screen.h"
#include "Plane.h"
#include "Mesh.h"

class Camera : public Object, public Animatable{
private:
    Point4D _angleLeftUpLookAt;

    Point4D _left =    Point4D{1, 0, 0, 0}; // internal X
    Point4D _up =      Point4D{0, 1, 0, 0}; // internal Y
    Point4D _lookAt =  Point4D{0, 0, 1, 0}; // internal Z

    Matrix4x4 _S; // screen space matrix
    Matrix4x4 _P; // projections matrix
    Matrix4x4 _V; // camera matrix

    double _aspect = 0;

    // To accelerate calculations we can use precalculated matrix that does not chance
    Matrix4x4 _SP; // screen-space-projections matrix

    std::vector<Triangle> _triangles;
    std::vector<Plane> _clipPlanes;

    bool _ready = false;

    double _w = 0;
    double _h = 0;
public:
    Camera() = default;
    Camera(const Camera& camera) = delete;

    void init(int width, int height, double fov = 110.0, double ZNear = 0.1, double ZFar = 5000.0);

    std::vector<Triangle>& project(std::shared_ptr<Mesh> mesh);

    void clear();

    [[nodiscard]] int buffSize() const { return _triangles.size(); }
    std::vector<Triangle>& sorted();

    [[nodiscard]] Point4D position() const override { return _position; }
    [[nodiscard]] Point4D angle() const override { return _angle; }
    [[nodiscard]] Point4D angleLeftUpLookAt() const { return _angleLeftUpLookAt; }

    [[nodiscard]] Point4D eye() const { return _position; }
    [[nodiscard]] Point4D left() const { return _left; }
    [[nodiscard]] Point4D right() const { return -_left; }
    [[nodiscard]] Point4D up() const { return _up; }
    [[nodiscard]] Point4D down() const { return -_up; }
    [[nodiscard]] Point4D lookAt() const { return _lookAt; }

    void translate(const Point4D& dv) override {
        _position = _position + dv;

        for(auto attached : _attachedObjects)
            attached->translate(dv);
    }

    void translateToPoint(const Point4D& point);

    void rotateX(double rx);
    void rotateY(double ry);
    void rotateZ(double rz);
    void rotate(const Point4D& r) override;

    void rotate(const Point4D& v, double rv) override;

    void rotateLeft(double rl);
    void rotateUp(double ru);
    void rotateLookAt(double rlAt);

    // Rotate body around XYZ by (r.x, r.y, r.z) radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& s, const Point4D& r) override;
    // Rotate body around normalised vector 'v' by 'r' radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& s, const Point4D& v, double r) override;
};


#endif //INC_3DZAVR_CAMERA_H
