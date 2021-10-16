//
// Created by Иван Ильин on 05.02.2021.
//

#include "RigidBody.h"
#include "../Plane.h"
#include "../utils/Log.h"
#include "../utils/Time.h"
#include <iostream>
#include <cmath>
#include <fstream>

Vec3D RigidBody::_findFurthestPoint(const Vec3D& direction) {
    std::shared_ptr<Vec3D> maxPoint = std::make_shared<Vec3D>(Vec3D{0, 0, 0});
    double maxDistance = -std::numeric_limits<double>::max();

    for(auto& tri : triangles()){
        for(int i = 0; i < 3; i++){
            Vec3D point = Vec3D(tri[i]) + position();

            double distance = point.dot(direction.normalized());
            if(distance > maxDistance) {
                maxDistance = distance;
                maxPoint = std::make_shared<Vec3D>(point);
            }
        }
    }

    return *maxPoint;
}

Vec3D RigidBody::_support(std::shared_ptr<RigidBody> obj, const Vec3D& direction) {
    Vec3D p1 = _findFurthestPoint(direction);
    Vec3D p2 = obj->_findFurthestPoint(-direction);
    Vec3D res = p1 - p2;

    return p1 - p2;
}

NextSimplex RigidBody::_nextSimplex(const Simplex &points) {
    switch (points.type()) {
        case SimplexType::Line: return _lineCase(points);
        case SimplexType::Triangle: return _triangleCase(points);
        case SimplexType::Tetrahedron: return _tetrahedronCase(points);

        default: throw std::logic_error{"RigidBody::_nextSimplex: simplex is not Line, Triangle or Tetrahedron"};
    }
}

NextSimplex RigidBody::_lineCase(const Simplex& points) {
    std::shared_ptr<Simplex> newPoints = std::make_shared<Simplex>(points);
    std::shared_ptr<Vec3D> newDirection;

    Vec3D a = points[0];
    Vec3D b = points[1];

    Vec3D ab = b - a;
    Vec3D ao =   - a;

    if (ab.dot(ao) > 0) {
        newDirection = std::make_shared<Vec3D>(ab.cross(ao).cross(ab));
    } else {
        newPoints = std::make_shared<Simplex>(Simplex{a});
        newDirection = std::make_shared<Vec3D>(ao);
    }

    return NextSimplex{*newPoints, *newDirection, false};
}

NextSimplex RigidBody::_triangleCase(const Simplex &points) {
    std::shared_ptr<Simplex> newPoints = std::make_shared<Simplex>(points);
    std::shared_ptr<Vec3D> newDirection;

    Vec3D a = points[0];
    Vec3D b = points[1];
    Vec3D c = points[2];

    Vec3D ab = b - a;
    Vec3D ac = c - a;
    Vec3D ao =   - a;

    Vec3D abc = ab.cross(ac);

    if (abc.cross(ac).dot(ao) > 0) {
        if (ac.dot(ao) > 0) {
            newPoints = std::make_shared<Simplex>(Simplex{ a, c });
            newDirection = std::make_shared<Vec3D>(ac.cross(ao).cross(ac));
        }
        else {
            return _lineCase(Simplex { a, b });
        }
    } else {
        if (ab.cross(abc).dot(ao) > 0) {
            return _lineCase(Simplex { a, b });
        }
        else {
            if (abc.dot(ao) > 0) {
                newDirection = std::make_shared<Vec3D>(abc);
            } else {
                newPoints = std::make_shared<Simplex>(Simplex{ a, c, b });
                newDirection = std::make_shared<Vec3D>(-abc);
            }
        }
    }

    return NextSimplex{*newPoints, *newDirection, false};
}

NextSimplex RigidBody::_tetrahedronCase(const Simplex &points) {
    Vec3D a = points[0];
    Vec3D b = points[1];
    Vec3D c = points[2];
    Vec3D d = points[3];

    Vec3D ab = b - a;
    Vec3D ac = c - a;
    Vec3D ad = d - a;
    Vec3D ao =   - a;

    Vec3D abc = ab.cross(ac);
    Vec3D acd = ac.cross(ad);
    Vec3D adb = ad.cross(ab);

    if (abc.dot(ao) > 0) {
        return _triangleCase(Simplex{ a, b, c });
    }

    if (acd.dot(ao) > 0) {
        return _triangleCase(Simplex{ a, c, d });
    }

    if (adb.dot(ao) > 0) {
        return _triangleCase(Simplex{ a, d, b });
    }

    return NextSimplex{points, Vec3D(), true};
}

std::pair<bool, Simplex> RigidBody::checkGJKCollision(std::shared_ptr<RigidBody> obj) {

    // Get initial support point in any direction
    std::shared_ptr<Vec3D> support = std::make_shared<Vec3D>(_support(obj, Vec3D{1, 0, 0}));

    // Simplex is an array of points, max count is 4
    std::shared_ptr<Simplex> points = std::make_shared<Simplex>();
    points->push_front(*support);

    // New direction is towards the origin
    std::shared_ptr<Vec3D> direction = std::make_shared<Vec3D>(-*support);

    while (true) {
        support = std::make_shared<Vec3D>(_support(obj, *direction));

        if (support->dot(*direction) <= 0)
            return std::make_pair(false, *points); // no collision

        points->push_front(*support);

        NextSimplex nextSimplex = _nextSimplex(*points);

        direction = std::make_shared<Vec3D>(nextSimplex.newDirection);
        points = std::make_shared<Simplex>(nextSimplex.newSimplex);

        if (nextSimplex.finishSearching) {
            if(obj->isCollider())
                _inCollision = true;
            return std::make_pair(true, *points);
        }
    }
}

CollisionPoint RigidBody::EPA(const Simplex& simplex, std::shared_ptr<RigidBody> obj) {

    std::vector<Vec3D> polytope(simplex.begin(), simplex.end());
    std::vector<size_t>  faces = {
            0, 1, 2,
            0, 3, 1,
            0, 2, 3,
            1, 3, 2
    };

    auto faceNormals = _getFaceNormals(polytope, faces);
    std::vector<std::shared_ptr<FaceNormal>> normals = faceNormals.first;
    size_t minFace = faceNormals.second;

    std::shared_ptr<Vec3D> minNormal = std::make_shared<Vec3D>(normals[minFace]->normal);
    double minDistance = std::numeric_limits<double>::max();

    int iters = 0;
    while (minDistance == std::numeric_limits<double>::max() && iters++ < size() + obj->size()) {
        minNormal = std::make_shared<Vec3D>(normals[minFace]->normal);
        minDistance = normals[minFace]->distance;

        Vec3D support = _support(obj, *minNormal);
        double sDistance = minNormal->dot(support);

        if (std::abs(sDistance - minDistance) > Consts::EPA_EPS) {
            minDistance = std::numeric_limits<double>::max();
            std::vector<std::pair<size_t, size_t>> uniqueEdges;

            for (size_t i = 0; i < normals.size(); i++) {
                if (normals[i]->normal.dot(support) > 0) {
                    size_t f = i * 3;

                    uniqueEdges = _addIfUniqueEdge(uniqueEdges, faces, f + 0, f + 1);
                    uniqueEdges = _addIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                    uniqueEdges = _addIfUniqueEdge(uniqueEdges, faces, f + 2, f + 0);

                    faces.erase(faces.begin() + f);
                    faces.erase(faces.begin() + f);
                    faces.erase(faces.begin() + f);
                    normals.erase(normals.begin() + i--);
                }
            }

            std::vector<size_t> newFaces;
            for (auto [edgeIndex1, edgeIndex2] : uniqueEdges) {
                newFaces.push_back(edgeIndex1);
                newFaces.push_back(edgeIndex2);
                newFaces.push_back(polytope.size());
            }
            polytope.push_back(support);

            faces.insert(faces.end(), newFaces.begin(), newFaces.end());

            auto newFaceNormals = _getFaceNormals(polytope, faces);

            normals = newFaceNormals.first;
            minFace = newFaceNormals.second;
        }
    }

    _collisionNormal = minNormal;
    if(std::abs(minDistance - std::numeric_limits<double>::max()) < Consts::EPS)
        return CollisionPoint{*minNormal, 0};

    return CollisionPoint{*minNormal, minDistance + Consts::EPA_EPS};
}

std::pair<std::vector<std::shared_ptr<FaceNormal>>, size_t> RigidBody::_getFaceNormals(const std::vector<Vec3D>& polytope, const std::vector<size_t>& faces) {
    std::vector<std::shared_ptr<FaceNormal>> normals;
    size_t nearestFaceIndex = 0;
    double minDistance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < faces.size(); i += 3) {
        Vec3D a = polytope[faces[i + 0]];
        Vec3D b = polytope[faces[i + 1]];
        Vec3D c = polytope[faces[i + 2]];

        std::shared_ptr<Vec3D> normal = std::make_shared<Vec3D>((b - a).cross(c - a).normalized());

        double distance = normal->dot(a);

        if (distance < -Consts::EPS) {
            normal = std::make_unique<Vec3D>(-*normal);
            distance *= -1;
        }

        normals.emplace_back(std::make_shared<FaceNormal>(FaceNormal{*normal, distance}));

        if (distance < minDistance) {
            nearestFaceIndex = i / 3;
            minDistance = distance;
        }
    }

    return {normals, nearestFaceIndex};
}

std::vector<std::pair<size_t, size_t>> RigidBody::_addIfUniqueEdge(const std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b) {

    std::vector<std::pair<size_t, size_t>> newEdges = edges;

    // We are interested in reversed edge
    //      0--<--3
    //     / \ B /   A: 2-0
    //    / A \ /    B: 0-2
    //   1-->--2
    auto reverse = std::find(newEdges.begin(), newEdges.end(), std::make_pair(faces[b], faces[a]));

    if (reverse != newEdges.end()) {
        newEdges.erase(reverse);
    } else {
        newEdges.emplace_back(faces[a], faces[b]);
    }

    return newEdges;
}

void RigidBody::updatePhysicsState() {
    translate(*_velocity * Time::deltaTime());
    _velocity = std::make_unique<Vec3D>(*_velocity + *_acceleration * Time::deltaTime());
}

void RigidBody::setVelocity(const Vec3D& velocity) {
    _velocity = std::make_unique<Vec3D>(velocity);
}

void RigidBody::addVelocity(const Vec3D &velocity) {
    _velocity = std::make_unique<Vec3D>(*_velocity + velocity);
}

void RigidBody::setAcceleration(const Vec3D& acceleration) {
    _acceleration = std::make_unique<Vec3D>(acceleration);
}

RigidBody::RigidBody(const Mesh &mesh) : Mesh(mesh) {

}

void RigidBody::makeLogObjPolytope(const std::vector<Vec3D> &polytope, const std::vector<size_t> &faces) {
    std::fstream file("polytope_log.obj", std::ios::out);

    for(auto &p : polytope)
        file << "v " << p.x() << " " << p.y()  << " " << p.z() << std::endl;

    for(int i = 0; i < faces.size(); i += 3)
        file << "f " << faces[i + 0]+1 << " " << faces[i + 1]+1  << " " << faces[i + 2]+1 << std::endl;

    file.close();
}
