//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <vector>
#include "Triangle.h"
#include "animation/Animatable.h"
#include "physics/RigidBody.h"
#include <SFML/Graphics.hpp>
#include "Object.h"

class Mesh : public Object, public Animatable, public RigidBody {
protected:
    std::vector<Triangle> tris;

    bool _visible = true;

    sf::Color c_color = sf::Color(255, 245, 194);

    // Operations with Matrix4x4
    [[nodiscard]] Mesh operator*(const Matrix4x4& matrix4X4) const;
    Mesh& operator*=(const Matrix4x4& matrix4X4);

    std::function<void(const std::string&, const std::shared_ptr<Mesh>&)> _collisionCallBack;

public:
    Mesh() = default;
    Mesh(const Mesh& mesh);

    explicit Mesh(const std::vector<Triangle>& tries);
    Mesh& operator=(const Mesh& mesh);
    explicit Mesh(const std::string& filename, const std::string &materials = "", const Point4D& scale = Point4D{1, 1, 1});

    Mesh& loadObj(const std::string& filename, const std::string &materials = "", const Point4D& scale = Point4D{1, 1, 1});

    [[nodiscard]] std::vector<Triangle>const &triangles() const { return tris; }
    [[nodiscard]] std::vector<Triangle>& triangles() override { return tris; }
    void setTriangles(const std::vector<Triangle>& t) override { tris = t; }

    // Translate mesh
    void translate(double dx, double dy, double dz);
    void translate(const Point4D& t) override;
    void translateToPoint(const Point4D& point);
    // Rotate mesh around XYZ axes
    void rotate(double rx, double ry, double rz);
    void rotate(const Point4D& r) override;
    // Rotate mesh around normalised vector 'v' by 'r' radians
    void rotate(const Point4D& v, double r) override;
    // Rotate mesh around XYZ by (rx, ry, rz) radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, double rx, double ry, double rz);
    // Rotate mesh around XYZ by (r.x, r.y, r.z) radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, const Point4D& r) override;
    // Rotate mesh around normalised vector 'v' by 'r' radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, const Point4D& v, double r) override;
    void scale(double sx, double sy, double sz);
    void scale(const Point4D& s);

    void rotateToAngle(const Point4D& v) { rotate(v - p_angle); }

    [[nodiscard]] Point4D position() const override { return p_position; }
    [[nodiscard]] Point4D angle() const override { return p_angle; }

    [[nodiscard]] sf::Color color() const override { return c_color; }
    void setColor(sf::Color c) override;

    Mesh static Cube(double size = 1.0);
    Mesh static Obj(const std::string& filename);
    Mesh static LineTo(const Point4D& from, const Point4D& to, double line_width = 0.1, sf::Color color = {150, 150, 150, 255});


    void setVisible(bool visibility) { _visible = visibility; }
    [[nodiscard]] bool isVisible() const { return _visible; }

    std::vector<std::shared_ptr<Mesh>> static LoadObjects(const std::string& filename, const std::string &materials = "", const Point4D& scale = Point4D{1, 1, 1});

    [[nodiscard]] const std::function<void(const std::string&, const std::shared_ptr<Mesh>&)>& collisionCallBack() const { return _collisionCallBack; }
    void setCollisionCallBack(const std::function<void(const std::string&, const std::shared_ptr<Mesh>&)>& f) { _collisionCallBack = f; }
};


#endif //INC_3DZAVR_MESH_H
