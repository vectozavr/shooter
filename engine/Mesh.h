//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <vector>
#include "Triangle.h"
#include <SFML/Graphics.hpp>
#include "Object.h"

class Mesh : public Object {
private:
    std::vector<Triangle> _tris;
    sf::Color _color = sf::Color(255, 245, 194);
    bool _visible = true;

    Mesh& operator*=(const Matrix4x4& matrix4X4);
public:
    Mesh() = default;
    Mesh& operator=(const Mesh& mesh) = delete;
    Mesh(const Mesh& mesh);

    explicit Mesh(const std::vector<Triangle>& tries);
    explicit Mesh(const std::string& filename, const Vec3D& scale = Vec3D{1, 1, 1});

    Mesh& loadObj(const std::string& filename, const Vec3D& scale = Vec3D{1, 1, 1});

    [[nodiscard]] std::vector<Triangle>const &triangles() const { return _tris; }
    [[nodiscard]] std::vector<Triangle>& triangles() { return _tris; }
    void setTriangles(const std::vector<Triangle>& t);

    // Translate body
    // Rotate body around XYZ axes
    void rotate(const Vec3D& r) override;
    // Rotate body around normalised vector 'v' by 'r' radians
    void rotate(const Vec3D& v, double r) override;
    // Rotate body around XYZ by (r._x, r._y, r.z) radians relative val 'point4D'
    void rotateRelativePoint(const Vec3D& point, const Vec3D& r) override;
    // Rotate body around normalised vector 'v' by 'r' radians relative val 'point4D'
    void rotateRelativePoint(const Vec3D& point4D, const Vec3D& v, double r) override;
    void scale(const Vec3D& s) override;
    [[nodiscard]] int size() const { return _tris.size()*3; }

    [[nodiscard]] sf::Color color() const { return _color; }
    void setColor(const sf::Color& c);

    void setVisible(bool visibility) { _visible = visibility; }
    [[nodiscard]] bool isVisible() const { return _visible; }

    ~Mesh() override;

    Mesh static Obj(const std::string& filename);
    Mesh static LineTo(const Vec3D& from, const Vec3D& to, double line_width = 0.1, const sf::Color& color = {150, 150, 150, 100});
};

#endif //INC_3DZAVR_MESH_H
