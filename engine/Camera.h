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
    Point4D p_angleLeftUpLookAt;

    Point4D p_left =    Point4D{1, 0, 0, 0}; // internal X
    Point4D p_up =      Point4D{0, 1, 0, 0}; // internal Y
    Point4D p_lookAt =  Point4D{0, 0, 1, 0}; // internal Z

    Matrix4x4 S; // screen space matrix
    Matrix4x4 P; // projections matrix
    Matrix4x4 V; // camera matrix

    double aspect = 0;

    // To accelerate calculations we can use precalculated matrix that does not chance
    Matrix4x4 SP; // screen-space-projections matrix
    Matrix4x4 VM; // camera-model-animation matrix

    std::vector<Triangle> triangles;
    std::vector<Plane> clipPlanes;

    bool ready = false;

    double w = 0;
    double h = 0;
public:
    Camera() = default;
    Camera(const Camera& camera) = delete;


    void init(int width, int height, double fov = 110.0, double ZNear = 0.1, double ZFar = 5000.0);

    std::vector<Triangle>& project(Mesh& mesh, Screen::ViewMode mode);

    void record();

    [[nodiscard]] int buffSize() const { return triangles.size(); }
    std::vector<Triangle>& sorted();

    [[nodiscard]] Point4D position() const override { return p_position; }
    [[nodiscard]] Point4D angle() const override { return p_angle; }
    [[nodiscard]] Point4D angleLeftUpLookAt() const { return p_angleLeftUpLookAt; }

    [[nodiscard]] Point4D eye() const { return p_position; }
    [[nodiscard]] Point4D left() const { return p_left; }
    [[nodiscard]] Point4D right() const { return -p_left; }
    [[nodiscard]] Point4D up() const { return p_up; }
    [[nodiscard]] Point4D down() const { return -p_up; }
    [[nodiscard]] Point4D lookAt() const { return p_lookAt; }

    void translate(const Point4D& dv) override {
        p_position = p_position + dv;

        for(auto attached : v_attached)
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

    // Rotate mesh around XYZ by (r.x, r.y, r.z) radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& s, const Point4D& r) override;
    // Rotate mesh around normalised vector 'v' by 'r' radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& s, const Point4D& v, double r) override;
};


#endif //INC_3DZAVR_CAMERA_H
