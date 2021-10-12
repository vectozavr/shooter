//
// Created by Иван Ильин on 15.03.2021.
//

#include "Object.h"
#include "Matrix4x4.h"
#include "utils/Log.h"

void Object::translate(const Vec3D &dv) {
    _position = std::make_unique<Vec3D>(*_position + dv);

    for(auto attached : _attachedObjects)
        attached.second->translate(dv);
}

void Object::scale(const Vec3D &s) {
    for(auto attached : _attachedObjects)
        attached.second->scale(s);
}

void Object::rotate(const Vec3D &r) {
    _angle = std::make_unique<Vec3D>(*_angle + r);

    Matrix4x4 rotationMatrix = Matrix4x4::RotationZ(r.z())*Matrix4x4::RotationY(r.y())*Matrix4x4::RotationX(r.z());

    _left = std::make_unique<Vec3D>(rotationMatrix * *_left);
    _up = std::make_unique<Vec3D>(rotationMatrix * *_up);
    _lookAt = std::make_unique<Vec3D>(rotationMatrix * *_lookAt);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), r);
}

void Object::rotate(const Vec3D &v, double rv) {
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, rv);

    _left = std::make_unique<Vec3D>(rotationMatrix * *_left);
    _up = std::make_unique<Vec3D>(rotationMatrix * *_up);
    _lookAt = std::make_unique<Vec3D>(rotationMatrix * *_lookAt);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), v, rv);
}

void Object::rotateRelativePoint(const Vec3D &s, const Vec3D &r) {
    _angle = std::make_unique<Vec3D>(*_angle + r);

    // Translate XYZ by vector r1
    Vec3D r1(*_position - s);

    // In translated coordinate system we rotate body and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(r);
    Vec3D r2(rotationMatrix*r1);

    _left = std::make_unique<Vec3D>(rotationMatrix * *_left);
    _up = std::make_unique<Vec3D>(rotationMatrix * *_up);
    _lookAt = std::make_unique<Vec3D>(rotationMatrix * *_lookAt);

    // After rotation we translate XYZ by vector -r2 and recalculate position
    _position = std::make_unique<Vec3D>(s + r2);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(s, r);
}

void Object::rotateRelativePoint(const Vec3D &s, const Vec3D &v, double r) {
    // Translate XYZ by vector r1
    Vec3D r1(*_position - s);
    // In translated coordinate system we rotate body and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, r);
    Vec3D r2 = rotationMatrix*r1;

    _left = std::make_unique<Vec3D>(rotationMatrix * *_left);
    _up = std::make_unique<Vec3D>(rotationMatrix * *_up);
    _lookAt = std::make_unique<Vec3D>(rotationMatrix * *_lookAt);

    // After rotation we translate XYZ by vector -r2 and recalculate position
    _position = std::make_unique<Vec3D>(s + r2);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(s, v, r);
}

void Object::rotateLeft(double rl) {
    _angleLeftUpLookAt = std::make_unique<Vec3D>(Vec3D{_angleLeftUpLookAt->x() + rl,
                                                          _angleLeftUpLookAt->y(),
                                                          _angleLeftUpLookAt->z()});

    rotate(*_left, rl);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), *_left, rl);
}

void Object::rotateUp(double ru) {
    _angleLeftUpLookAt = std::make_unique<Vec3D>(Vec3D{_angleLeftUpLookAt->x(),
                                                          _angleLeftUpLookAt->y() + ru,
                                                          _angleLeftUpLookAt->z()});
    rotate(*_up, ru);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), *_up, ru);
}

void Object::rotateLookAt(double rlAt) {
    _angleLeftUpLookAt = std::make_unique<Vec3D>(Vec3D{_angleLeftUpLookAt->x(),
                                                          _angleLeftUpLookAt->y(),
                                                          _angleLeftUpLookAt->z() + rlAt});
    rotate(*_lookAt, rlAt);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), *_lookAt, rlAt);
}

void Object::translateToPoint(const Vec3D &point) {
    translate(point - *_position);
}

void Object::rotateToAngle(const Vec3D &v) {
    rotate(v - *_angle);
}

std::shared_ptr<Object> Object::attached(const std::string &name) {
    if(_attachedObjects.count(name) == 0)
        Log::log("Object::attached: object '" + name + "' does not exist.");
    return _attachedObjects.find(name)->second;
}

void Object::attach(std::shared_ptr<Object> object, const std::string &name) {
    // TODO: solve problem with possible infinite recursive call chains
    if(this != object.get())
        _attachedObjects.emplace(name, object);
    else
        throw std::invalid_argument{"Object::attach: You cannot attach object to itself"};
}

void Object::unattach(const std::string &name) {
    _attachedObjects.erase(name);
}
