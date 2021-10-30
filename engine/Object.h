//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include <map>
#include "Vec3D.h"
#include <string>
#include <utility>
#include <memory>
#include "Matrix4x4.h"
#include <SFML/OpenGL.hpp>

class ObjectNameTag final {
private:
    const std::string _name;
public:
    explicit ObjectNameTag(std::string name = "") : _name(std::move(name)) {}
    [[nodiscard]] std::string str() const { return _name; }

    bool operator==(const ObjectNameTag& tag) const { return _name == tag._name; }
    bool operator!=(const ObjectNameTag& tag) const { return _name != tag._name; }
    bool operator<(const ObjectNameTag& tag) const { return _name < tag._name; }
};

class Object {
private:
    bool checkIfAttached(Object* obj);
    const ObjectNameTag _nameTag;

    Matrix4x4 _transformMatrix = Matrix4x4::Identity();

    std::map<ObjectNameTag, std::weak_ptr<Object>> _attachedObjects;

    Vec3D _position         {0, 0, 0};
    Vec3D _angle            {0, 0, 0};
    Vec3D _angleLeftUpLookAt{0, 0, 0};
public:
    explicit Object(ObjectNameTag nameTag) : _nameTag(std::move(nameTag)) {};
    Object(const Object& object) : _nameTag(object.name()), _transformMatrix(object.model()) {};

    // TODO: add transform(const Matrix4x4& t)
    // TODO: add transformRelativePoint(const Vec3D &point, const Matrix4x4& transform)
    // TODO: implement every transform method using transform() & transformRelativePoint()
    // TODO: implement rotations using quaternions
    void translate(const Vec3D& dv);
    void translateToPoint(const Vec3D& point);
    void scale(const Vec3D& s);
    void rotate(const Vec3D& r);
    void rotate(const Vec3D& v, double rv);
    void rotateToAngle(const Vec3D& v);
    void rotateRelativePoint(const Vec3D& s, const Vec3D& r);
    void rotateRelativePoint(const Vec3D& s, const Vec3D& v, double r);
    void rotateLeft(double rl);
    void rotateUp(double ru);
    void rotateLookAt(double rlAt);

    [[nodiscard]] Vec3D left() const        { return _transformMatrix.x(); }
    [[nodiscard]] Vec3D up() const          { return _transformMatrix.y(); }
    [[nodiscard]] Vec3D lookAt() const      { return _transformMatrix.z(); }
    [[nodiscard]] Vec3D position() const    { return _position; }

    [[nodiscard]] Vec3D angle() const { return _angle; }
    [[nodiscard]] Vec3D angleLeftUpLookAt() const { return _angleLeftUpLookAt; }

    void attach(std::shared_ptr<Object> object);
    void unattach(const ObjectNameTag& tag);
    std::shared_ptr<Object> attached(const ObjectNameTag& tag);

    [[nodiscard]] ObjectNameTag name() const { return _nameTag; }

    [[nodiscard]] Matrix4x4 model() const { return Matrix4x4::Translation(_position) * _transformMatrix; }

    // OpenGL function
    [[nodiscard]] GLfloat* glModel() const;
    [[nodiscard]] GLfloat* glView() const;

    virtual ~Object();
};

#endif //MINECRAFT_3DZAVR_OBJECT_H
