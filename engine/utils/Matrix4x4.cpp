//
// Created by Иван Ильин on 12.01.2021.
//

#include "Matrix4x4.h"
#include <cassert>
#include <cmath>

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &matrix4X4) const {
    Matrix4x4 result = Matrix4x4::Zero();

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            for(int k = 0; k < 4; k++)
                result._arr[i][j] += _arr[i][k] * matrix4X4._arr[k][j];
    return result;
}

Point4D Matrix4x4::operator*(const Point4D &point4D) const {
    return Point4D(
            _arr[0][0] * point4D.x() + _arr[0][1] * point4D.y() + _arr[0][2] * point4D.z() + _arr[0][3] * point4D.w(),
            _arr[1][0] * point4D.x() + _arr[1][1] * point4D.y() + _arr[1][2] * point4D.z() + _arr[1][3] * point4D.w(),
            _arr[2][0] * point4D.x() + _arr[2][1] * point4D.y() + _arr[2][2] * point4D.z() + _arr[2][3] * point4D.w(),
            _arr[3][0] * point4D.x() + _arr[3][1] * point4D.y() + _arr[3][2] * point4D.z() + _arr[3][3] * point4D.w()
    );
}

Matrix4x4 Matrix4x4::Identity() {
    Matrix4x4 result;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(i == j)
                result._arr[j][i] = 1.0;
            else
                result._arr[j][i] = 0.0;

    return result;
}

Matrix4x4 Matrix4x4::Constant(double value) {
    Matrix4x4 result;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            result._arr[j][i] = value;

    return result;
}

Matrix4x4 Matrix4x4::Zero() {
    return Matrix4x4::Constant(0);
}

Matrix4x4 Matrix4x4::Scale(const Point4D& factor) {
    Matrix4x4 s{};
    s._arr[0][0] = factor.x();
    s._arr[1][1] = factor.y();
    s._arr[2][2] = factor.z();
    s._arr[3][3] = 1;

    return s;
}

Matrix4x4 Matrix4x4::Translation(const Point4D& v) {
    Matrix4x4 t{};
/*
 * ( 1 0 0 dx )(_x)   (_x + dx)
 * ( 0 1 0 dy )(_y) = (_y + dy)
 * ( 0 0 1 dz )(z)   (z + dz)
 * ( 0 0 0 1  )(1)   (  1   )
 */

    t._arr[0][0] = 1.0;
    t._arr[1][1] = 1.0;
    t._arr[2][2] = 1.0;
    t._arr[3][3] = 1.0;

    t._arr[0][3] = v.x();
    t._arr[1][3] = v.y();
    t._arr[2][3] = v.z();

    return t;
}

Matrix4x4 Matrix4x4::RotationX(double rx) {
    Matrix4x4 Rx{};
    Rx._arr[0][0] = 1.0;

    Rx._arr[1][1] = cos(rx);
    Rx._arr[1][2] = -sin(rx);
    Rx._arr[2][1] = sin(rx);
    Rx._arr[2][2] = cos(rx);

    Rx._arr[3][3] = 1.0;

    return Rx;
}

Matrix4x4 Matrix4x4::RotationY(double ry) {
    Matrix4x4 Ry{};

    Ry._arr[1][1] = 1.0;

    Ry._arr[0][0] = cos(ry);
    Ry._arr[0][2] = sin(ry);
    Ry._arr[2][0] = -sin(ry);
    Ry._arr[2][2] = cos(ry);

    Ry._arr[3][3] = 1.0;

    return Ry;
}

Matrix4x4 Matrix4x4::RotationZ(double rz) {
    Matrix4x4 Rz{};

    Rz._arr[2][2] = 1.0;

    Rz._arr[0][0] = cos(rz);
    Rz._arr[0][1] = -sin(rz);
    Rz._arr[1][0] = sin(rz);
    Rz._arr[1][1] = cos(rz);

    Rz._arr[3][3] = 1.0;

    return Rz;
}

Matrix4x4 Matrix4x4::Rotation(const Point4D& r) {
    return RotationX(r.x()) * RotationY(r.y()) * RotationZ(r.z());
}

Matrix4x4 Matrix4x4::Rotation(Point4D v, double rv) {
    Matrix4x4 Rv{};
    v = v.normalized();

    Rv._arr[0][0] = cos(rv) + (1.0 - cos(rv))*v.x()*v.x();
    Rv._arr[0][1] = (1.0 - cos(rv))*v.x()*v.y() - sin(rv)*v.z();
    Rv._arr[0][2] = (1.0 - cos(rv))*v.x()*v.z() + sin(rv)*v.y();

    Rv._arr[1][0] = (1.0 - cos(rv))*v.x()*v.y() + sin(rv)*v.z();
    Rv._arr[1][1] = cos(rv) + (1.0 - cos(rv))*v.y()*v.y();
    Rv._arr[1][2] = (1.0 - cos(rv))*v.y()*v.z() - sin(rv)*v.x();

    Rv._arr[2][0] = (1.0 - cos(rv))*v.z()*v.x() - sin(rv)*v.y();
    Rv._arr[2][1] = (1.0 - cos(rv))*v.z()*v.y() + sin(rv)*v.x();
    Rv._arr[2][2] = cos(rv) + (1.0 - cos(rv))*v.z()*v.z();

    Rv._arr[3][3] = 1.0;

    return Rv;
}

Matrix4x4 Matrix4x4::Projection(double fov, double aspect, double ZNear, double ZFar) {
    Matrix4x4 p{};

    p._arr[0][0] = 1.0/(tan(M_PI*fov*0.5/180.0)*aspect);
    p._arr[1][1] = 1.0/tan(M_PI*fov*0.5/180.0);
    p._arr[2][2] = ZFar/(ZFar - ZNear);
    p._arr[2][3] = -ZFar*ZNear/(ZFar - ZNear);
    p._arr[3][2] = 1.0;

    return p;
}

Matrix4x4 Matrix4x4::ScreenSpace(int width, int height) {
    Matrix4x4 s{};

    s._arr[0][0] = -0.5*width;
    s._arr[1][1] = -0.5*height;
    s._arr[2][2] = 1.0;

    s._arr[0][3] = 0.5*width;
    s._arr[1][3] = 0.5*height;

    s._arr[3][3] = 1.0;

    return s;
}

Matrix4x4 Matrix4x4::View(const Point4D &left, const Point4D &up, const Point4D &lookAt, const Point4D &eye) {
    Matrix4x4 V = Zero();

    V._arr[0][0] = left[0];
    V._arr[0][1] = left[1];
    V._arr[0][2] = left[2];
    V._arr[0][3] = -eye.dot(left);

    V._arr[1][0] = up[0];
    V._arr[1][1] = up[1];
    V._arr[1][2] = up[2];
    V._arr[1][3] = -eye.dot(up);

    V._arr[2][0] = lookAt[0];
    V._arr[2][1] = lookAt[1];
    V._arr[2][2] = lookAt[2];
    V._arr[2][3] = -eye.dot(lookAt);

    V._arr[3][3] = 1.0;

    return V;
}
