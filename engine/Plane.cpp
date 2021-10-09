//
// Created by Иван Ильин on 19.01.2021.
//

#include "Plane.h"

Plane::Plane(const Triangle& tri) {
    _triangle = tri;
    _n = tri.norm();
    _p = tri[0];
}

Plane::Plane(const Point4D &N, const Point4D &P) {
    _n = N.normalized();
    _p = P;
}

Plane::Plane(const Plane &plane) {
    _triangle = plane._triangle;
    _n = plane._n;
    _p = plane._p;
}

double Plane::distance(const Point4D &point4D) const {
    return point4D.dot(_n) - _p.dot(_n);
}

std::pair<Point4D, double> Plane::intersection(const Point4D &start, const Point4D &end) const {
    double s_dot_n = start.dot(_n);
    double k = (s_dot_n - _p.dot(_n)) / (s_dot_n - end.dot(_n));
    Point4D res = start + (end - start)*k;
    return std::make_pair(res, k);
}

int Plane::clip(Triangle &tri, Triangle &additional_tri) const {

    Point4D insidePoints[3];  int inside = 0;
    Point4D outsidePoints[3]; int outside = 0;

    double distances[3] = {distance(tri[0]), distance(tri[1]), distance(tri[2])};

    for(int i = 0; i < 3; i++) {
        if (distances[i] >= 0) {
            insidePoints[inside++] = tri[i];
        } else {
            outsidePoints[outside++] = tri[i];
        }
    }

    if(inside == 0) {
        return 0;
    }

    if(inside == 1) {
        std::pair<Point4D, double> intersect1 = intersection(insidePoints[0], outsidePoints[0]);
        std::pair<Point4D, double> intersect2 = intersection(insidePoints[0], outsidePoints[1]);

        tri = Triangle(insidePoints[0], intersect1.first, intersect2.first, tri.color());

        return 1;
    }

    if(inside == 2) {
        std::pair<Point4D, double> intersect1 = intersection(insidePoints[0], outsidePoints[0]);
        std::pair<Point4D, double> intersect2 = intersection(insidePoints[1], outsidePoints[0]);

        tri = Triangle(insidePoints[0], intersect1.first, insidePoints[1], tri.color());
        additional_tri = Triangle(intersect1.first, intersect2.first, insidePoints[1], tri.color());

        return 2;
    }

    if(inside == 3) {
        return 1;
    }

    return 0;
}
