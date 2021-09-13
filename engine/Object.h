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
    std::vector<std::shared_ptr<Object>> v_attached;

    Point4D p_position;
    Point4D p_angle;
public:
    Object() = default;

    virtual void translate(const Point4D& dv) {}
    virtual void rotate(const Point4D& r) {}
    virtual void rotateRelativePoint(const Point4D& point4D, const Point4D& r) {}
    virtual void rotate(const Point4D& v, double rv) {}
    virtual void rotateRelativePoint(const Point4D& s, const Point4D& v, double r) {}

    [[nodiscard]] Point4D position() const { return p_position; }
    [[nodiscard]] Point4D angle() const { return p_angle; }

    void attach(const std::shared_ptr<Object>& object) {
        v_attached.push_back(object);
    }
};


#endif //MINECRAFT_3DZAVR_OBJECT_H
