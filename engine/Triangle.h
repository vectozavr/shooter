//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_TRIANGLE_H
#define ENGINE_TRIANGLE_H

#include "utils/Point4D.h"
#include "utils/Matrix4x4.h"
#include <SFML/Graphics.hpp>

class Triangle {
private:
    sf::Color _color;
    Point4D _p[3];

public:

    Triangle ();
    Triangle (const Triangle& triangle);
    Triangle (const Point4D& p1, const Point4D& p2, const Point4D& p3, sf::Color color = {0, 0, 0});

    [[nodiscard]] Point4D operator[] (int i) const;
    [[nodiscard]] Point4D norm() const;

    // Operations with Matrix4x4
    [[nodiscard]] Triangle operator*(const Matrix4x4& matrix4X4) const;

    [[nodiscard]] bool isPointInside(const Point4D& point) const;

    [[nodiscard]] sf::Color color() const { return _color; }
};


#endif //INC_3DZAVR_TRIANGLE_H
