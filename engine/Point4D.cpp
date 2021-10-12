//
// Created by Иван Ильин on 12.01.2021.
//

#include "Point4D.h"
#include <cmath>
#include <stdexcept>

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

[[nodiscard]] Point4D Point4D::operator-() const {
    return Point4D(-x(), -y(), -z(), -w());
}

bool Point4D::operator==(const Point4D& point4D) const {
    return (*this - point4D).sqrAbs() < Consts::EPS;
}

bool Point4D::operator!=(const Point4D& point4D) const {
    return !(*this == point4D);
}

// Operations with Point4D
Point4D Point4D::operator+(const Point4D& point4D) const {
    return Point4D(x()+point4D.x(), y()+point4D.y(), z()+point4D.z(), w()+point4D.w());
}

Point4D Point4D::operator-(const Point4D& point4D) const {
    return Point4D(*this) + -point4D;
}

Point4D Point4D::operator*(double number) const {
    return Point4D(x()*number, y()*number, z()*number, w()*number);
}

Point4D Point4D::operator/(double number) const {
    if(std::abs(number) > Consts::EPS)
        return Point4D(*this)*(1.0/number);
    else
        throw std::domain_error{"Point4D::operator/(double number): division by zero"};
}

// Other useful methods
double Point4D::sqrAbs() const {
    return x()*x() + y()*y() + z()*z();
}

double Point4D::abs() const {
    return sqrt(sqrAbs());
}

Point4D Point4D::normalized() const {
    double vecAbs = abs();
    if(vecAbs > Consts::EPS)
        return Point4D(*this)/abs();
    else
        return Point4D(0);
}
