//
// Created by Иван Ильин on 15.03.2021.
//

#include "Object.h"
#include "utils/Matrix4x4.h"
#include "utils/Log.h"

void Object::translate(const Point4D &dv) {
    _position = _position + dv;

    for(auto attached : _attachedObjects)
        attached.second->translate(dv);
}

void Object::scale(const Point4D &s) {
    for(auto attached : _attachedObjects)
        attached.second->scale(s);
}

void Object::rotate(const Point4D &r) {
    _angle = _angle + r;

    Matrix4x4 rotationMatrix = Matrix4x4::RotationZ(r.z())*Matrix4x4::RotationY(r.y())*Matrix4x4::RotationX(r.z());

    _left = rotationMatrix * _left;
    _up = rotationMatrix * _up;
    _lookAt = rotationMatrix * _lookAt;

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), r);
}

void Object::rotate(const Point4D &v, double rv) {
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, rv);
    _left = rotationMatrix * _left;
    _up = rotationMatrix * _up;
    _lookAt = rotationMatrix * _lookAt;

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), v, rv);
}

void Object::rotateRelativePoint(const Point4D &s, const Point4D &r) {
    _angle = _angle + r;

    // Translate XYZ by vector r1
    Point4D r1 = _position - s;

    // In translated coordinate system we rotate body and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(r);
    Point4D r2 = rotationMatrix*r1;

    _left = rotationMatrix * _left;
    _up = rotationMatrix * _up;
    _lookAt = rotationMatrix * _lookAt;

    // After rotation we translate XYZ by vector -r2 and recalculate position
    _position = s + r2;

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(s, r);
}

void Object::rotateRelativePoint(const Point4D &s, const Point4D &v, double r) {
    // Translate XYZ by vector r1
    Point4D r1 = _position - s;
    // In translated coordinate system we rotate body and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, r);
    Point4D r2 = rotationMatrix*r1;

    _left = rotationMatrix * _left;
    _up = rotationMatrix * _up;
    _lookAt = rotationMatrix * _lookAt;

    // After rotation we translate XYZ by vector -r2 and recalculate position
    _position = s + r2;

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(s, v, r);
}

void Object::rotateLeft(double rl) {
    _angleLeftUpLookAt = Point4D{_angleLeftUpLookAt.x() + rl, _angleLeftUpLookAt.y(), _angleLeftUpLookAt.z()};

    rotate(_left, rl);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), _left, rl);
}

void Object::rotateUp(double ru) {
    _angleLeftUpLookAt = Point4D{_angleLeftUpLookAt.x(), _angleLeftUpLookAt.y() + ru, _angleLeftUpLookAt.z()};
    rotate(_up, ru);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), _up, ru);
}

void Object::rotateLookAt(double rlAt) {
    _angleLeftUpLookAt = Point4D{_angleLeftUpLookAt.x(), _angleLeftUpLookAt.y(), _angleLeftUpLookAt.z() + rlAt};
    rotate(_lookAt, rlAt);

    for(auto attached : _attachedObjects)
        attached.second->rotateRelativePoint(position(), _lookAt, rlAt);
}

void Object::translateToPoint(const Point4D &point) {
    translate(point - _position);
}

void Object::rotateToAngle(const Point4D &v) {
    rotate(v - _angle);
}

std::shared_ptr<Object> Object::attached(const std::string &name) {
    if(_attachedObjects.count(name) == 0)
        Log::log("Object::attached: object '" + name + "' does not exist.");
    return _attachedObjects.find(name)->second;
}

void Object::attach(std::shared_ptr<Object> object, const std::string &name) {
    _attachedObjects.emplace(name, object);
}

void Object::unattach(const std::string &name) {
    _attachedObjects.erase(name);
}
