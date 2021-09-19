//
// Created by Иван Ильин on 05.02.2021.
//

#ifndef ENGINE_RIGIDBODY_H
#define ENGINE_RIGIDBODY_H

#include <vector>
#include <memory>
#include "../utils/Point4D.h"
#include "../Triangle.h"
#include "Simplex.h"
#include "Mesh.h"

struct CollisionPoint {
    Point4D a; // Furthest point of a into b
    Point4D b; // Furthest point of b into a
    Point4D normal; // b – a normalized
    double depth;    // Length of b – a
    bool hasCollision;
};

class RigidBody : public Mesh {
private:
    Point4D _findFurthestPoint(const Point4D& direction);
    Point4D _support(std::shared_ptr<RigidBody> obj, const Point4D& direction);

    std::function<void(const std::string&, std::shared_ptr<RigidBody>)> _collisionCallBack;

    static bool _nextSimplex(Simplex& points, Point4D& direction);
    static bool _line(Simplex& points, Point4D& direction);
    static bool _triangle(Simplex& points, Point4D& direction);
    static bool _tetrahedron(Simplex& points, Point4D& direction);

    static std::pair<std::vector<Point4D>, size_t> GetFaceNormals(const std::vector<Point4D>& polytope, const std::vector<size_t>&  faces);
    static void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);

protected:
    Point4D _velocity;
    Point4D _acceleration;

    bool _collision = false;
    bool _isCollider = true;

    bool _inCollision = false;
    Point4D _collisionNormal;

public:
    RigidBody() = default;
    explicit RigidBody(const Mesh& mesh);

    std::pair<bool, Simplex> checkGJKCollision(std::shared_ptr<RigidBody> obj);
    CollisionPoint EPA(const Simplex& simplex, std::shared_ptr<RigidBody> obj);

    [[nodiscard]] bool isCollision() const { return _collision; }
    [[nodiscard]] bool inCollision() const {return _inCollision; }
    [[nodiscard]] bool isCollider() const {return _isCollider; }
    void setInCollision(bool c) { _inCollision = c; }
    void setCollisionNormal(const Point4D& c) { _collisionNormal = c; }
    void setCollision(bool c) { _collision= c; }
    void setCollider(bool c) { _isCollider = c; }

    void updatePhysicsState();

    void setVelocity(const Point4D& velocity);
    void addVelocity(const Point4D& velocity);
    void setAcceleration(const Point4D& acceleration);

    [[nodiscard]] Point4D velocity() const { return _velocity; }

    [[nodiscard]] const std::function<void(const std::string&, std::shared_ptr<RigidBody>)>& collisionCallBack() const { return _collisionCallBack; }
    void setCollisionCallBack(const std::function<void(const std::string&, std::shared_ptr<RigidBody>)>& f) { _collisionCallBack = f; }
};


#endif //INC_3DZAVR_RIGIDBODY_H
