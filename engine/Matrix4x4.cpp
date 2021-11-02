//
// Created by Иван Ильин on 12.01.2021.
//

#include <cmath>

#include "Matrix4x4.h"
#include "Consts.h"

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &matrix4X4) const {
    Matrix4x4 result = Matrix4x4::Zero();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                result._arr[i][j] += _arr[i][k] * matrix4X4._arr[k][j];
    return result;
}

Vec4D Matrix4x4::operator*(const Vec4D &point4D) const {
    return Vec4D(
            _arr[0][0] * point4D.x() + _arr[0][1] * point4D.y() + _arr[0][2] * point4D.z() + _arr[0][3] * point4D.w(),
            _arr[1][0] * point4D.x() + _arr[1][1] * point4D.y() + _arr[1][2] * point4D.z() + _arr[1][3] * point4D.w(),
            _arr[2][0] * point4D.x() + _arr[2][1] * point4D.y() + _arr[2][2] * point4D.z() + _arr[2][3] * point4D.w(),
            _arr[3][0] * point4D.x() + _arr[3][1] * point4D.y() + _arr[3][2] * point4D.z() + _arr[3][3] * point4D.w()
    );
}

Vec3D Matrix4x4::operator*(const Vec3D &vec) const {
    return Vec3D(
            _arr[0][0] * vec.x() + _arr[0][1] * vec.y() + _arr[0][2] * vec.z(),
            _arr[1][0] * vec.x() + _arr[1][1] * vec.y() + _arr[1][2] * vec.z(),
            _arr[2][0] * vec.x() + _arr[2][1] * vec.y() + _arr[2][2] * vec.z()
    );
}

// TODO: please, separate rotation and scale. Zero scale can mess up inverse matrix
Matrix4x4 Matrix4x4::inversed() const {
    Matrix4x4 result = Matrix4x4::Identity();
    Matrix4x4 temp(*this);

    for (size_t k = 0; k < _arr.size(); k++) {
        double mult = temp._arr[k][k];

        for (size_t j = 0; j < _arr.size(); j++) {
            temp._arr[k][j] /= mult;
            result._arr[k][j] /= mult;
        }

        for (size_t i = k + 1; i < _arr.size(); i++) {
            mult = temp._arr[i][k];

            for (size_t j = 0; j < _arr.size(); j++) {
                temp._arr[i][j] -= temp._arr[k][j] * mult;
                result._arr[i][j] -= result._arr[k][j] * mult;
            }
        }
    }

    for (size_t k = _arr.size() - 1; k > 0; k--) {
        for (size_t i = 0; i < k; i++) {
            double mult = temp._arr[i][k];

            for (size_t j = 0; j < _arr.size(); j++) {
                temp._arr[i][j] -= temp._arr[k][j] * mult;
                result._arr[i][j] -= result._arr[k][j] * mult;
            }
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::Identity() {
    Matrix4x4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                result._arr[j][i] = 1.0;
            } else {
                result._arr[j][i] = 0.0;
            }
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::Constant(double value) {
    Matrix4x4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result._arr[j][i] = value;
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::Zero() {
    return Matrix4x4::Constant(0);
}

Matrix4x4 Matrix4x4::Scale(const Vec3D &factor) {
    Matrix4x4 s{};
    s._arr[0][0] = factor.x();
    s._arr[1][1] = factor.y();
    s._arr[2][2] = factor.z();
    s._arr[3][3] = 1;

    return s;
}

Matrix4x4 Matrix4x4::Translation(const Vec3D &v) {
    Matrix4x4 t{};

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

Matrix4x4 Matrix4x4::Rotation(const Vec3D &r) {
    return RotationX(r.x()) * RotationY(r.y()) * RotationZ(r.z());
}

Matrix4x4 Matrix4x4::Rotation(const Vec3D &v, double rv) {
    Matrix4x4 Rv{};
    Vec3D nv(v.normalized());

    Rv._arr[0][0] = cos(rv) + (1.0 - cos(rv)) * nv.x() * nv.x();
    Rv._arr[0][1] = (1.0 - cos(rv)) * nv.x() * nv.y() - sin(rv) * nv.z();
    Rv._arr[0][2] = (1.0 - cos(rv)) * nv.x() * nv.z() + sin(rv) * nv.y();

    Rv._arr[1][0] = (1.0 - cos(rv)) * nv.x() * nv.y() + sin(rv) * nv.z();
    Rv._arr[1][1] = cos(rv) + (1.0 - cos(rv)) * nv.y() * nv.y();
    Rv._arr[1][2] = (1.0 - cos(rv)) * nv.y() * nv.z() - sin(rv) * nv.x();

    Rv._arr[2][0] = (1.0 - cos(rv)) * nv.z() * nv.x() - sin(rv) * nv.y();
    Rv._arr[2][1] = (1.0 - cos(rv)) * nv.z() * nv.y() + sin(rv) * nv.x();
    Rv._arr[2][2] = cos(rv) + (1.0 - cos(rv)) * nv.z() * nv.z();

    Rv._arr[3][3] = 1.0;

    return Rv;
}

Matrix4x4 Matrix4x4::Projection(double fov, double aspect, double ZNear, double ZFar) {
    Matrix4x4 p{};

    p._arr[0][0] = 1.0 / (tan(Consts::PI * fov * 0.5 / 180.0) * aspect);
    p._arr[1][1] = 1.0 / tan(Consts::PI * fov * 0.5 / 180.0);
    p._arr[2][2] = ZFar / (ZFar - ZNear);
    p._arr[2][3] = -ZFar * ZNear / (ZFar - ZNear);
    p._arr[3][2] = 1.0;

    return p;
}

Matrix4x4 Matrix4x4::ScreenSpace(int width, int height) {
    Matrix4x4 s{};

    s._arr[0][0] = -0.5 * width;
    s._arr[1][1] = -0.5 * height;
    s._arr[2][2] = 1.0;

    s._arr[0][3] = 0.5 * width;
    s._arr[1][3] = 0.5 * height;

    s._arr[3][3] = 1.0;

    return s;
}

Matrix4x4 Matrix4x4::View(const Vec3D &left, const Vec3D &up, const Vec3D &lookAt, const Vec3D &eye) {
    Matrix4x4 V = Zero();

    V._arr[0][0] = left.x();
    V._arr[0][1] = left.y();
    V._arr[0][2] = left.z();
    V._arr[0][3] = -eye.dot(left);

    V._arr[1][0] = up.x();
    V._arr[1][1] = up.y();
    V._arr[1][2] = up.z();
    V._arr[1][3] = -eye.dot(up);

    V._arr[2][0] = lookAt.x();
    V._arr[2][1] = lookAt.y();
    V._arr[2][2] = lookAt.z();
    V._arr[2][3] = -eye.dot(lookAt);

    V._arr[3][3] = 1.0;

    return V;
}

Vec3D Matrix4x4::x() const {
    return Vec3D(_arr[0][0], _arr[1][0], _arr[2][0]);
}

Vec3D Matrix4x4::y() const {
    return Vec3D(_arr[0][1], _arr[1][1], _arr[2][1]);
}

Vec3D Matrix4x4::z() const {
    return Vec3D(_arr[0][2], _arr[1][2], _arr[2][2]);
}

Vec3D Matrix4x4::w() const {
    return Vec3D(_arr[0][3], _arr[1][3], _arr[2][3]);
}
