//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include <map>
#include "utils/Point4D.h"
#include <memory>

class Object {
protected:
    Point4D _left =   Point4D{1, 0, 0, 0}; // internal X
    Point4D _up =     Point4D{0, 1, 0, 0}; // internal Y
    Point4D _lookAt = Point4D{0, 0, 1, 0}; // internal Z

    std::map<std::string, std::shared_ptr<Object>> _attachedObjects;

    Point4D _position;
    Point4D _angle;
    Point4D _angleLeftUpLookAt;
public:
    Object() = default;

    virtual void translate(const Point4D& dv);
    virtual void translateToPoint(const Point4D& point);
    virtual void scale(const Point4D& s);
    virtual void rotate(const Point4D& r);
    virtual void rotate(const Point4D& v, double rv);
    virtual void rotateToAngle(const Point4D& v);
    virtual void rotateRelativePoint(const Point4D& s, const Point4D& r);
    virtual void rotateRelativePoint(const Point4D& s, const Point4D& v, double r);

    [[nodiscard]] Point4D position() const { return _position; }
    [[nodiscard]] Point4D angle() const { return _angle; }
    [[nodiscard]] Point4D angleLeftUpLookAt() const { return _angleLeftUpLookAt; }

    [[nodiscard]] Point4D left() const { return _left; }
    [[nodiscard]] Point4D up() const { return _up; }
    [[nodiscard]] Point4D lookAt() const { return _lookAt; }

    void rotateLeft(double rl);
    void rotateUp(double ru);
    void rotateLookAt(double rlAt);

    void attach(std::shared_ptr<Object> object, const std::string& name);
    void unattach(const std::string& name);
    std::shared_ptr<Object> attached(const std::string& name);
};

#endif //MINECRAFT_3DZAVR_OBJECT_H
