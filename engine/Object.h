//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include <vector>
#include "utils/Point4D.h"
#include <memory>

class Object {
protected:
    std::vector<std::shared_ptr<Object>> _attachedObjects;

    Point4D _position;
    Point4D _angle;
public:
    Object() = default;

    virtual void translate(const Point4D& dv) {}
    virtual void rotate(const Point4D& r) {}
    virtual void rotateRelativePoint(const Point4D& point4D, const Point4D& r) {}
    virtual void rotate(const Point4D& v, double rv) {}
    virtual void rotateRelativePoint(const Point4D& s, const Point4D& v, double r) {}

    [[nodiscard]] Point4D position() const { return _position; }
    [[nodiscard]] Point4D angle() const { return _angle; }

    void attach(std::shared_ptr<Object> object) {
        _attachedObjects.push_back(object);
    }
};


#endif //MINECRAFT_3DZAVR_OBJECT_H
