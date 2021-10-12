//
// Created by Иван Ильин on 13.01.2021.
//

#include "Triangle.h"

Triangle::Triangle () {
    _points.emplace_back(Point4D(0, 0, 0, 1));
    _points.emplace_back(Point4D(0, 0, 0, 1));
    _points.emplace_back(Point4D(0, 0, 0, 1));
}

Triangle::Triangle(const Point4D& p1, const Point4D& p2, const Point4D& p3, sf::Color color) : _color(color) {
    _points.emplace_back(Point4D(p1));
    _points.emplace_back(Point4D(p2));
    _points.emplace_back(Point4D(p3));
}

Triangle::Triangle(const Triangle &triangle) : _points(triangle._points), _color(triangle._color) {

}

Triangle Triangle::operator*(const Matrix4x4 &matrix4X4) const {
    return Triangle(matrix4X4 * _points[0], matrix4X4 * _points[1], matrix4X4 * _points[2], _color);
}

Vec3D Triangle::norm() const {

    Vec3D v1 = Vec3D(_points[1] - _points[0]);
    Vec3D v2 = Vec3D(_points[2] - _points[0]);
    Vec3D crossProduct = v1.cross(v2);

    if(crossProduct.sqrAbs() > Consts::EPS)
        return crossProduct.normalized();
    else
        return Vec3D(0);
}

Point4D Triangle::operator[](int i) const {
    return _points[i];
}

bool Triangle::isPointInside(const Vec3D &point) const {
    Vec3D triangleNorm = norm();

    double dot1 = (point - Vec3D(_points[0])).cross(Vec3D(_points[1] - _points[0])).dot(triangleNorm);
    double dot2 = (point - Vec3D(_points[1])).cross(Vec3D(_points[2] - _points[1])).dot(triangleNorm);
    double dot3 = (point - Vec3D(_points[2])).cross(Vec3D(_points[0] - _points[2])).dot(triangleNorm);

    if((dot1 >= 0 && dot2 >= 0 && dot3 >= 0) || (dot1 <= 0 && dot2 <= 0 && dot3 <= 0))
        return true;
    return false;
}
