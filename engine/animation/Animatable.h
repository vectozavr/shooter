//
// Created by Иван Ильин on 26.01.2021.
//

#ifndef ENGINE_ANIMATABLE_H
#define ENGINE_ANIMATABLE_H

#include <list>
#include "../Triangle.h"

//class Animation;
#include "Animation.h"
#include <iostream>
#include <functional>

// All _objects in 3dzavr that should be animated must inherit class Animatable:
class Animatable {
protected:
    std::map<std::string, std::list<Animation*>> animations;

public:
    Animatable() = default;
    virtual ~Animatable() = default;
    // All methods about animation begins with 'a_'
    void a_translate(const std::string& listName,
                     const Point4D& t,
                     double duration = 1,
                     Animation::LoopOut looped = Animation::None,
                     Animation::InterpolationType interpolationType = Animation::bezier);
    void a_translateToPoint(const std::string& listName,
                            const Point4D& point,
                            double duration = 1,
                            Animation::LoopOut looped = Animation::None,
                            Animation::InterpolationType interpolationType = Animation::bezier);

    void a_rotate(const std::string& listName,
                  const Point4D& r,
                  double duration = 1,
                  Animation::LoopOut looped = Animation::None,
                  Animation::InterpolationType interpolationType = Animation::bezier);

    void a_scale(const std::string& listName,
                 const Point4D& s,
                 double duration = 1,
                 Animation::LoopOut looped = Animation::None,
                 Animation::InterpolationType interpolationType = Animation::bezier);

    void a_wait(const std::string& listName, double duration = 1);

    void a_function(const std::string& listName,
                    std::function<void()> function,
                    int calls = 1,
                    double duration = 1,
                    Animation::LoopOut looped = Animation::None,
                    Animation::InterpolationType interpolationType = Animation::bezier);

    void a_color(const std::string& listName,
                 const sf::Color& color,
                 double duration = 1,
                 Animation::LoopOut looped = Animation::None,
                 Animation::InterpolationType interpolationType = Animation::bezier);


    void a_update();

    void a_stopAllAnimations() { animations.clear(); }
    void a_stopAnimationList(const std::string& name) { animations[name].clear(); }

    [[nodiscard]] bool isInAnim() const {
        for(auto& animList : animations)
            if (!animList.second.empty())
                return true;
        return false;

    }
    [[nodiscard]] bool isInAnimList(const std::string& name) { return !animations[name].empty(); }

    // methods to override:
    // If you want to create new animation you can either add new virtual function here
    // or override one of the following function:
    [[nodiscard]] virtual Point4D position() const { return Point4D{}; }
    [[nodiscard]] virtual Point4D angle() const { return Point4D{}; }
    virtual void translate(const Point4D& dv) {}
    virtual void rotate(const Point4D& r) {}
    [[nodiscard]] virtual std::vector<Triangle>& triangles() { return *(std::vector<Triangle>*)(new std::vector<Triangle>()); }
    virtual void setTriangles(const std::vector<Triangle>& tris) {}

    [[nodiscard]] virtual sf::Color color() const {return sf::Color(); }
    virtual void setColor(sf::Color c) { }
};

#endif //INC_3DZAVR_ANIMATABLE_H
