//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_TRIANGLE_H
#define ENGINE_TRIANGLE_H

#include "Vec4D.h"
#include "Vec3D.h"
#include "Matrix4x4.h"
#include <SFML/Graphics.hpp>

class Triangle final {
private:
    sf::Color _color;
    Vec4D _points[3];
public:
    Triangle() : _points{Vec4D{}, Vec4D{}, Vec4D{}} {};
    Triangle(const Triangle& triangle);
    Triangle(const Vec4D& p1, const Vec4D& p2, const Vec4D& p3, sf::Color color = {0, 0, 0});
    Triangle& operator=(const Triangle&) = default;

    [[nodiscard]] Vec4D operator[] (int i) const;
    [[nodiscard]] Vec3D norm() const;

    // Operations with Matrix4x4
    [[nodiscard]] Triangle operator*(const Matrix4x4& matrix4X4) const;

    [[nodiscard]] bool isPointInside(const Vec3D& point) const;

    [[nodiscard]] sf::Color color() const { return _color; }

    [[nodiscard]] double distance(const Vec3D& vec) const { return norm().dot(Vec3D(_points[0]) - vec); }
};


#endif //INC_3DZAVR_TRIANGLE_H
