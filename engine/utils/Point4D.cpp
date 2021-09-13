//
// Created by Иван Ильин on 12.01.2021.
//

#include "Point4D.h"
#include <cmath>

Point4D::Point4D (double x, double y, double z, double w) {
    _arr_point[0] = x;
    _arr_point[1] = y;
    _arr_point[2] = z;
    _arr_point[3] = w;
}

Point4D::Point4D(const Point4D &point4D) {
    _arr_point[0] = point4D.x();
    _arr_point[1] = point4D.y();
    _arr_point[2] = point4D.z();
    _arr_point[3] = point4D.w();
}

Point4D &Point4D::operator=(const Point4D &point4D) {
    if (&point4D != this)
    {
        _arr_point[0] = point4D.x();
        _arr_point[1] = point4D.y();
        _arr_point[2] = point4D.z();
        _arr_point[3] = point4D.w();
    }
    return *this;
}

[[nodiscard]] double Point4D::operator[] (int i) const {
    return _arr_point[i];
}
[[nodiscard]] double& Point4D::operator[] (int i) {
    return _arr_point[i];
}

[[nodiscard]] Point4D Point4D::operator-() const {
    return Point4D(-x(), -y(), -z(), -w());
}
[[nodiscard]] Point4D Point4D::operator+() const {
    return Point4D(*this);
}

// Boolean operations
bool Point4D::operator==(const Point4D& point4D) const
{
    return this == &point4D || (*this - point4D).sqrAbs() < 0.0000000001;
}
bool Point4D::operator!=(const Point4D& point4D) const
{
    return this != &point4D && (*this - point4D).sqrAbs() > 0.0000000001;
}

// Operations with Point4D
Point4D Point4D::operator+(const Point4D& point4D) const
{
    return Point4D(*this) += point4D;
}
Point4D Point4D::operator-(const Point4D& point4D) const
{
    return Point4D(*this) -= point4D;
}
Point4D Point4D::operator*(const Point4D& point4D) const
{
    return Point4D(*this) *= point4D;
}
Point4D Point4D::operator/(const Point4D& point4D) const
{
    return Point4D(*this) /= point4D;
}

Point4D& Point4D::operator+=(const Point4D& point4D)
{
    _arr_point[0] += point4D.x();
    _arr_point[1] += point4D.y();
    _arr_point[2] += point4D.z();
    _arr_point[3] += point4D.w();
    return *this;
}
Point4D& Point4D::operator-=(const Point4D& point4D)
{
    (*this) += -point4D;
    return *this;
}
Point4D& Point4D::operator*=(const Point4D& point4D) {
    _arr_point[0] *= point4D.x();
    _arr_point[1] *= point4D.y();
    _arr_point[2] *= point4D.z();
    _arr_point[3] *= point4D.w();
    return *this;
}

Point4D& Point4D::operator/=(const Point4D& point4D) {
    _arr_point[0] /= point4D.x();
    _arr_point[1] /= point4D.y();
    _arr_point[2] /= point4D.z();
    _arr_point[3] /= point4D.w();
    return *this;
}
double Point4D::dot(const Point4D& point4D) const
{
    return point4D.x() * x() + point4D.y() * y() + point4D.z() * z() + point4D.w() * w();
}
[[nodiscard]] Point4D Point4D::cross3D(const Point4D& point4D) const {
    Point4D cross{y() * point4D.z() - point4D.y() * z(),
                  z() * point4D.x() - point4D.z() * x(),
                  x() * point4D.y() - point4D.x() * y()};
    return cross;
}

// Operations with numbers
Point4D Point4D::operator+(double number) const {
    return Point4D(*this) += number;
}
Point4D Point4D::operator-(double number) const {
    return Point4D(*this) -= number;
}
Point4D Point4D::operator*(double number) const
{
    return Point4D(*this) *= number;
}
Point4D Point4D::operator/(double number) const
{
    return Point4D(*this) /= number;
}
Point4D& Point4D::operator+=(double number) {
    _arr_point[0] += number;
    _arr_point[1] += number;
    _arr_point[2] += number;
    _arr_point[3] += number;
    return *this;
}
Point4D& Point4D::operator-=(double number) {
    return *this += -number;
}
Point4D& Point4D::operator*=(double number)
{
    _arr_point[0] *= number;
    _arr_point[1] *= number;
    _arr_point[2] *= number;
    _arr_point[3] *= number;
    return *this;
}
Point4D& Point4D::operator/=(double number)
{
    return *this *= 1.0/number;
}

// Other useful methods
double Point4D::sqrAbs() const
{
    return x()*x() + y()*y() + z()*z() + w()*w();
}
double Point4D::abs() const
{
    return sqrt(sqrAbs());
}
Point4D& Point4D::normalize()
{
    double length = this->abs();
    _arr_point[0] /= length;
    _arr_point[1] /= length;
    _arr_point[2] /= length;
    _arr_point[3] /= length;
    return *this;
}

Point4D Point4D::normalized() const {
    Point4D res(*this);

    return res.normalize();
}
