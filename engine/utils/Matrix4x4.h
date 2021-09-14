//
// Created by Иван Ильин on 12.01.2021.
//

#ifndef ENGINE_MATRIX4X4_H
#define ENGINE_MATRIX4X4_H

#include <array>
#include "Point4D.h"

class Matrix4x4 {
private:
    std::array<std::array<double, 4>, 4> _arr{};
public:
    Matrix4x4 () = default;
    Matrix4x4& operator=(const Matrix4x4& matrix4X4) = default;

    [[nodiscard]] Matrix4x4 operator*(const Matrix4x4& matrix4X4) const;
    [[nodiscard]] Point4D operator*(const Point4D& point4D) const;

    // Any useful matrix (static methods)
    Matrix4x4 static Identity();
    Matrix4x4 static Zero();
    Matrix4x4 static Constant (double value);

    Matrix4x4 static Scale(const Point4D& factor);
    Matrix4x4 static Translation(const Point4D& v);
    Matrix4x4 static Rotation(const Point4D& r);
    Matrix4x4 static RotationX (double rx);
    Matrix4x4 static RotationY (double ry);
    Matrix4x4 static RotationZ (double rz);
    Matrix4x4 static Rotation (Point4D v, double rv);

    Matrix4x4 static View(const Point4D& left, const Point4D& up, const Point4D& lookAt, const Point4D& eye);
    Matrix4x4 static Projection (double fov = 90.0, double aspect = 1.0, double ZNear = 1.0, double ZFar = 10.0);
    Matrix4x4 static ScreenSpace (int width, int height);
};


#endif //INC_3DZAVR_MATRIX4X4_H
