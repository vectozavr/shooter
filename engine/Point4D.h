//
// Created by Иван Ильин on 12.01.2021.
//

#ifndef ENGINE_POINT4D_H
#define ENGINE_POINT4D_H

#include <array>
#include "Consts.h"

class Point4D {
private:
    std::array<double, 4> _arr_point{};

public:
    Point4D () = default;
    Point4D (const Point4D& point4D);
    explicit Point4D (double x, double y = 0.0, double z = 0.0, double w = 0.0);
    Point4D& operator=(const Point4D& point4D) = delete;


    [[nodiscard]] double x() const { return _arr_point[0]; }
    [[nodiscard]] double y() const { return _arr_point[1]; }
    [[nodiscard]] double z() const { return _arr_point[2]; }
    [[nodiscard]] double w() const { return _arr_point[3]; }

    [[nodiscard]] Point4D operator-() const;

    // Boolean operations
    bool operator==(const Point4D& point4D) const;
    bool operator!=(const Point4D& point4D) const;

    // Operations with Point4D
    [[nodiscard]] Point4D operator+(const Point4D& point4D) const;
    [[nodiscard]] Point4D operator-(const Point4D& point4D) const;

    // Operations with numbers
    [[nodiscard]] Point4D operator*(double number) const;
    [[nodiscard]] Point4D operator/(double number) const;

    // Other useful methods
    [[nodiscard]] double sqrAbs() const; // Returns squared vector length
    [[nodiscard]] double abs() const; // Returns vector length
    [[nodiscard]] Point4D normalized() const; // Returns normalized vector without changing
};

#endif //INC_3DZAVR_POINT4D_H
