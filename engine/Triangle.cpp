//
// Created by Иван Ильин on 13.01.2021.
//

#include "Triangle.h"

Triangle::Triangle () {
    _p[0] = Point4D{0, 0, 0, 1};
    _p[1] = Point4D{0, 0, 0, 1};
    _p[2] = Point4D{0, 0, 0, 1};
}

Triangle::Triangle(const Point4D& p1, const Point4D& p2, const Point4D& p3, sf::Color color) {
    _p[0] = p1;
    _p[1] = p2;
    _p[2] = p3;
    _color = color;
}

Triangle Triangle::operator*(const Matrix4x4 &matrix4X4) const {
    Triangle res(*this);

    res._p[0] = matrix4X4 * _p[0];
    res._p[1] = matrix4X4 * _p[1];
    res._p[2] = matrix4X4 * _p[2];

    return res;
}

Point4D Triangle::norm() const {

    Point4D v1 = _p[1] - _p[0];
    Point4D v2 = _p[2] - _p[0];

    return v1.cross3D(v2).normalized();
}

Point4D Triangle::operator[](int i) const {
    return _p[i];
}

Triangle::Triangle(const Triangle &triangle) {
    _color = triangle._color;
    _p[0] = triangle[0];
    _p[1] = triangle[1];
    _p[2] = triangle[2];
}

bool Triangle::isPointInside(const Point4D &point) const {
    Point4D triangleNorm = norm();

    double dot1 = (point - _p[0]).cross3D(_p[1] - _p[0]).dot(triangleNorm);
    double dot2 = (point - _p[1]).cross3D(_p[2] - _p[1]).dot(triangleNorm);
    double dot3 = (point - _p[2]).cross3D(_p[0] - _p[2]).dot(triangleNorm);

    if((dot1 >= 0 && dot2 >= 0 && dot3 >= 0) || (dot1 <= 0 && dot2 <= 0 && dot3 <= 0))
        return true;
    return false;
}
