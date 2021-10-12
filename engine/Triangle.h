//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_TRIANGLE_H
#define ENGINE_TRIANGLE_H

#include "Point4D.h"
#include "Vec3D.h"
#include "Matrix4x4.h"
#include <SFML/Graphics.hpp>

class Triangle {
private:
    sf::Color _color;
    std::vector<Point4D> _points;
public:

    Triangle ();
    Triangle (const Triangle& triangle);
    Triangle (const Point4D& p1, const Point4D& p2, const Point4D& p3, sf::Color color = {0, 0, 0});
    Triangle& operator=(const Triangle&) = delete;

    [[nodiscard]] Point4D operator[] (int i) const;
    [[nodiscard]] Vec3D norm() const;

    // Operations with Matrix4x4
    [[nodiscard]] Triangle operator*(const Matrix4x4& matrix4X4) const;

    [[nodiscard]] bool isPointInside(const Vec3D& point) const;

    [[nodiscard]] sf::Color color() const { return _color; }

};


#endif //INC_3DZAVR_TRIANGLE_H
