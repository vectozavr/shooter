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

    _transformMatrix = Matrix4x4::Scale(s)*_transformMatrix;

    for(auto &[attachedName, attachedObject] : _attachedObjects) {
        if(!attachedObject.expired()) {
            attachedObject.lock()->scale(s);
        }
    }
}

void Object::rotate(const Vec3D &r) {
    _angle = _angle + r;

    Matrix4x4 rotationMatrix = Matrix4x4::RotationZ(r.z())*Matrix4x4::RotationY(r.y())*Matrix4x4::RotationX(r.z());

    _transformMatrix = rotationMatrix*_transformMatrix;

    for(auto &[attachedName, attachedObject] : _attachedObjects) {
        if(!attachedObject.expired()) {
            attachedObject.lock()->rotateRelativePoint(position(), r);
        }
    }
}

void Object::rotate(const Vec3D &v, double rv) {
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, rv);

    _transformMatrix = rotationMatrix*_transformMatrix;

    for(auto &[attachedName, attachedObject] : _attachedObjects) {
        if(!attachedObject.expired()) {
            attachedObject.lock()->rotateRelativePoint(position(), v, rv);
        }
    }
}

void Object::rotateRelativePoint(const Vec3D &s, const Vec3D &r) {
    _angle = _angle + r;

    // Translate XYZ by vector r1
    Vec3D r1(position() - s);

    // In translated coordinate system we rotate body and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(r);
    Vec3D r2(rotationMatrix*r1);

    _transformMatrix = rotationMatrix*_transformMatrix;

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
    Vec3D r1(position() - s);
    // In translated coordinate system we rotate body and position
    Matrix4x4 rotationMatrix = Matrix4x4::Rotation(v, r);
    Vec3D r2 = rotationMatrix*r1;

    _transformMatrix = rotationMatrix*_transformMatrix;

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

    rotate(left(), rl);
}

void Object::rotateUp(double ru) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y() + ru,
                               _angleLeftUpLookAt.z()};

    rotate(up(), ru);
}

void Object::rotateLookAt(double rlAt) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y(),
                               _angleLeftUpLookAt.z() + rlAt};
    rotate(lookAt(), rlAt);
}

void Object::translateToPoint(const Vec3D &point) {
    translate(point - position());
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

// OpenGL function
GLfloat* Object::glView() const {
    auto* v = (GLfloat*)malloc(4*4*sizeof(GLfloat));

    v[0] = -(GLfloat)left().x();
    v[4] = -(GLfloat)left().y();
    v[8] = -(GLfloat)left().z();
    v[12] = (GLfloat)position().dot(left());

    v[1] = (GLfloat)up().x();
    v[5] = (GLfloat)up().y();
    v[9] = (GLfloat)up().z();
    v[13] = -(GLfloat)position().dot(up());

    v[2] = -(GLfloat)lookAt().x();
    v[6] = -(GLfloat)lookAt().y();
    v[10] = -(GLfloat)lookAt().z();
    v[14] = (GLfloat)position().dot(lookAt());

    v[3] = (GLfloat)0.0f;
    v[7] = (GLfloat)0.0f;
    v[11] = (GLfloat)0.0f;
    v[15] = (GLfloat)1.0f;

    return v;
}

GLfloat* Object::glModel() const {
    auto* m = (GLfloat*)malloc(4*4*sizeof(GLfloat));

    m[0] = (GLfloat)left().x();
    m[4] = (GLfloat)up().x();
    m[8] = (GLfloat)lookAt().x();
    m[12] = (GLfloat)position().x();

    m[1] = (GLfloat)left().y();
    m[5] = (GLfloat)up().y();
    m[9] = (GLfloat)lookAt().y();
    m[13] = (GLfloat)position().y();

    m[2] = (GLfloat)left().z();
    m[6] = (GLfloat)up().z();
    m[10] =(GLfloat)lookAt().z();
    m[14] = (GLfloat)position().z();

    m[3] = (GLfloat)0.0f;
    m[7] = (GLfloat)0.0f;
    m[11] = (GLfloat)0.0f;
    m[15] = (GLfloat)1.0f;

    return m;
}

Object::~Object() {
    _attachedObjects.clear();
}
