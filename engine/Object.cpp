//
// Created by Иван Ильин on 15.03.2021.
//

#include "Object.h"
#include "Matrix4x4.h"
#include "utils/Log.h"

void Object::translate(const Vec3D &dv) {
    _position = _position + dv;

    for(auto &[attachedName, attachedObject] : _attachedObjects) {
        if(!attachedObject.expired()) {
            attachedObject.lock()->translate(dv);
        }
    }
}

void Object::scale(const Vec3D &s) {
    for(auto &[attachedName, attachedObject] : _attachedObjects) {
        if(!attachedObject.expired()) {
            attachedObject.lock()->scale(s);
        }
    }
}

void Object::rotate(const Vec3D &r) {
    _angle = _angle + r;

    Matrix4x4 rotationMatrix = Matrix4x4::RotationZ(r.z())*Matrix4x4::RotationY(r.y())*Matrix4x4::RotationX(r.z());

    _left = rotationMatrix * _left;
    _up = rotationMatrix * _up;
    _lookAt = rotationMatrix * _lookAt;

    for(auto &[attachedName, attachedObject] : _attachedObjects) {
        if(!attachedObject.expired()) {
            attachedObject.lock()->rotateRelativePoint(position(), r);
        }
    }
}

void Object::rotate(const Vec3D &v, double rv) {
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, rv);

    _left = rotationMatrix * _left;
    _up = rotationMatrix * _up;
    _lookAt = rotationMatrix * _lookAt;

    for(auto &[attachedName, attachedObject] : _attachedObjects) {
        if(!attachedObject.expired()) {
            attachedObject.lock()->rotateRelativePoint(position(), v, rv);
        }
    }
}

void Object::rotateRelativePoint(const Vec3D &s, const Vec3D &r) {
    _angle = _angle + r;

    // Translate XYZ by vector r1
    Vec3D r1(_position - s);

    // In translated coordinate system we rotate body and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(r);
    Vec3D r2(rotationMatrix*r1);

    _left = rotationMatrix * _left;
    _up = rotationMatrix * _up;
    _lookAt = rotationMatrix * _lookAt;

    // After rotation we translate XYZ by vector -r2 and recalculate position
    _position = s + r2;

    for(auto &[attachedName, attachedObject] : _attachedObjects) {
        if(!attachedObject.expired()) {
            attachedObject.lock()->rotateRelativePoint(s, r);
        }
    }
}

void Object::rotateRelativePoint(const Vec3D &s, const Vec3D &v, double r) {
    // Translate XYZ by vector r1
    Vec3D r1(_position - s);
    // In translated coordinate system we rotate body and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, r);
    Vec3D r2 = rotationMatrix*r1;

    _left = rotationMatrix * _left;
    _up = rotationMatrix * _up;
    _lookAt = rotationMatrix * _lookAt;

    // After rotation we translate XYZ by vector -r2 and recalculate position
    _position = s + r2;

    for(auto &[attachedName, attachedObject] : _attachedObjects) {
        if(!attachedObject.expired()) {
            attachedObject.lock()->rotateRelativePoint(s, v, r);
        }
    }
}

void Object::rotateLeft(double rl) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x() + rl,
                               _angleLeftUpLookAt.y(),
                               _angleLeftUpLookAt.z()};

    rotate(Vec3D(_left), rl);
}

void Object::rotateUp(double ru) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y() + ru,
                               _angleLeftUpLookAt.z()};

    rotate(Vec3D(_up), ru);
}

void Object::rotateLookAt(double rlAt) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y(),
                               _angleLeftUpLookAt.z() + rlAt};
    rotate(Vec3D(_lookAt), rlAt);
}

void Object::translateToPoint(const Vec3D &point) {
    translate(point - _position);
}

void Object::rotateToAngle(const Vec3D &v) {
    rotate(v - _angle);
}

std::shared_ptr<Object> Object::attached(const ObjectNameTag& tag) {
    if(_attachedObjects.count(tag) == 0 || _attachedObjects.find(tag)->second.expired()) {
        return nullptr;
    }
    return _attachedObjects.find(tag)->second.lock();
}

bool Object::checkIfAttached(Object *obj) {
    for(const auto& [nameTag, attachedObject] : _attachedObjects) {
        if (obj == attachedObject.lock().get() || attachedObject.lock()->checkIfAttached(obj)) {
            return true;
        }
    }
    return false;
}

void Object::attach(std::shared_ptr<Object> object) {
    if(this != object.get()) {
        if(!object->checkIfAttached(this)) {
            _attachedObjects.emplace(object->name(), object);
        } else {
            throw std::invalid_argument{"Object::attach: You tried to create infinite recursive call chains"};
        }
    } else {
        throw std::invalid_argument{"Object::attach: You cannot attach object to itself"};
    }
}

void Object::unattach(const ObjectNameTag& tag) {
    _attachedObjects.erase(tag);
}

Object::~Object() {
    _attachedObjects.clear();
}
