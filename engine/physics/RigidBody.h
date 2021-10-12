//
// Created by Иван Ильин on 05.02.2021.
//

#ifndef ENGINE_RIGIDBODY_H
#define ENGINE_RIGIDBODY_H

#include <vector>
#include <memory>
#include <functional>
#include "../Triangle.h"
#include "Simplex.h"
#include "../Mesh.h"

struct CollisionPoint {
    const Vec3D normal;
    const double depth;
    const bool hasCollision;
};

struct FaceNormal {
    const Vec3D normal;
    const double distance;
};

struct NextSimplex {
    const Simplex newSimplex;
    const Vec3D newDirection;
    const bool finishSearching;
};

class RigidBody : public Mesh {
private:
    Vec3D _findFurthestPoint(const Vec3D& direction);
    Vec3D _support(std::shared_ptr<RigidBody> obj, const Vec3D& direction);

    std::function<void(const std::string&, std::shared_ptr<RigidBody>)> _collisionCallBack;

    static NextSimplex _nextSimplex(const Simplex& points);
    static NextSimplex _lineCase(const Simplex& points);
    static NextSimplex _triangleCase(const Simplex& points);
    static NextSimplex _tetrahedronCase(const Simplex& points);

    static std::pair<std::vector<std::shared_ptr<FaceNormal>>, size_t> _getFaceNormals(const std::vector<Vec3D>& polytope, const std::vector<size_t>&  faces);
    static std::vector<std::pair<size_t, size_t>> _addIfUniqueEdge(const std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);

protected:
    std::unique_ptr<Vec3D> _velocity = std::make_unique<Vec3D>(Vec3D{0, 0, 0});;
    std::unique_ptr<Vec3D> _acceleration = std::make_unique<Vec3D>(Vec3D{0, 0, 0});;

    bool _collision = false;
    bool _isCollider = true;

    bool _inCollision = false;
    std::shared_ptr<Vec3D> _collisionNormal = std::make_unique<Vec3D>(Vec3D{0, 0, 0});;

public:
    RigidBody() = default;
    explicit RigidBody(const Mesh& mesh);

    [[nodiscard]] std::pair<bool, Simplex> checkGJKCollision(std::shared_ptr<RigidBody> obj);
    [[nodiscard]] CollisionPoint EPA(const Simplex& simplex, std::shared_ptr<RigidBody> obj);

    [[nodiscard]] Vec3D collisionNormal() const { return *_collisionNormal; }

    [[nodiscard]] bool isCollision() const { return _collision; }
    [[nodiscard]] bool inCollision() const {return _inCollision; }
    [[nodiscard]] bool isCollider() const {return _isCollider; }
    void setInCollision(bool c) { _inCollision = c; }
    void setCollision(bool c) { _collision= c; }
    void setCollider(bool c) { _isCollider = c; }

    void updatePhysicsState();

    void setVelocity(const Vec3D& velocity);
    void addVelocity(const Vec3D& velocity);
    void setAcceleration(const Vec3D& acceleration);

    [[nodiscard]] Vec3D velocity() const { return *_velocity; }
    [[nodiscard]] Vec3D acceleration() const { return *_acceleration; }

    [[nodiscard]] const std::function<void(const std::string&, std::shared_ptr<RigidBody>)>& collisionCallBack() const { return _collisionCallBack; }
    void setCollisionCallBack(const std::function<void(const std::string&, std::shared_ptr<RigidBody>)>& f) { _collisionCallBack = f; }
};


#endif //INC_3DZAVR_RIGIDBODY_H
