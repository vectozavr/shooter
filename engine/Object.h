//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include <map>
#include "Vec3D.h"
#include <string>
#include <utility>

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
protected:
    std::unique_ptr<Vec3D> _left   = std::make_unique<Vec3D>(Vec3D{1, 0, 0}); // internal X
    std::unique_ptr<Vec3D> _up     = std::make_unique<Vec3D>(Vec3D{0, 1, 0}); // internal Y
    std::unique_ptr<Vec3D> _lookAt = std::make_unique<Vec3D>(Vec3D{0, 0, 1}); // internal Z

    std::map<ObjectNameTag, std::shared_ptr<Object>> _attachedObjects;

    std::unique_ptr<Vec3D> _position = std::make_unique<Vec3D>(Vec3D{0, 0, 0});
    std::unique_ptr<Vec3D> _angle = std::make_unique<Vec3D>(Vec3D{0, 0, 0});
    std::unique_ptr<Vec3D> _angleLeftUpLookAt = std::make_unique<Vec3D>(Vec3D{0, 0, 0});
public:
    Object() = default;

    virtual void translate(const Vec3D& dv);
    virtual void translateToPoint(const Vec3D& point);
    virtual void scale(const Vec3D& s);
    virtual void rotate(const Vec3D& r);
    virtual void rotate(const Vec3D& v, double rv);
    virtual void rotateToAngle(const Vec3D& v);
    virtual void rotateRelativePoint(const Vec3D& s, const Vec3D& r);
    virtual void rotateRelativePoint(const Vec3D& s, const Vec3D& v, double r);
    void rotateLeft(double rl);
    void rotateUp(double ru);
    void rotateLookAt(double rlAt);

    [[nodiscard]] Vec3D position() const { return *_position; }
    [[nodiscard]] Vec3D angle() const { return *_angle; }
    [[nodiscard]] Vec3D angleLeftUpLookAt() const { return *_angleLeftUpLookAt; }

    [[nodiscard]] Vec3D left() const { return *_left; }
    [[nodiscard]] Vec3D up() const { return *_up; }
    [[nodiscard]] Vec3D lookAt() const { return *_lookAt; }

    void attach(std::shared_ptr<Object> object, const ObjectNameTag& tag);
    void unattach(const ObjectNameTag& tag);
    std::shared_ptr<Object> attached(const ObjectNameTag& tag);

    virtual ~Object();
};

#endif //MINECRAFT_3DZAVR_OBJECT_H
