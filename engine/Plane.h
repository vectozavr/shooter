//
// Created by Иван Ильин on 19.01.2021.
//

#ifndef ENGINE_PLANE_H
#define ENGINE_PLANE_H

#include "Point4D.h"
#include "Triangle.h"

class Plane final {
private:
    // You can define plane by defining the points in 3D space
    Triangle _triangle;
    // Or by defining normal vector and one val laying on the plane
    Vec3D _n = Vec3D{0, 0, 1};
    Vec3D _p{};
public:
    // A plain with normal vector '_n' and val '_p' lays on the plane
    Plane() = default;
    Plane(const Vec3D& N, const Vec3D& P);
    Plane(const Plane& plane) = default;
    explicit Plane(const Triangle& tri);

    [[nodiscard]] double distance(const Vec3D& point4D) const;
    // Point4D in space where line ('start' to 'end') intersects plain with normal vector '_n' and val '_p' lays on the plane
    [[nodiscard]] std::pair<Vec3D, double> intersection(const Vec3D& start, const Vec3D& end) const;
    [[nodiscard]] std::vector<Triangle> clip(const Triangle& tri) const;

    [[nodiscard]] Vec3D N() const { return _n; }
    [[nodiscard]] Vec3D P() const { return _p; }
};


#endif //INC_3DZAVR_PLANE_H
