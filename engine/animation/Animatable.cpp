//
// Created by Иван Ильин on 26.01.2021.
//

#include "Animatable.h"
#include <iostream>
#include <utility>

#include "ATranslate.h"
#include "ATranslateToPoint.h"
#include "ARotate.h"
#include "AScale.h"
#include "AWait.h"
#include "AFunction.h"
#include "AColor.h"

/*
void Animatable::a_translate(const std::string& listName,
                             const Point4D &t,
                             double duration,
                             Animation::LoopOut looped,
                             Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ATranslate(t, duration, looped, interpolationType));
}

void Animatable::a_translateToPoint(const std::string& listName,
                                    const Point4D &point,
                                    double duration,
                                    Animation::LoopOut looped,
                                    Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ATranslateToPoint(point, duration, looped, interpolationType));
}

void Animatable::a_rotate(c
                          const Point4D &r,
                          double duration,
                          Animation::LoopOut looped,
                          Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ARotate(r, duration, looped, interpolationType));
}

void Animatable::a_scale(const std::string& listName,
                         const Point4D &s,
                         double duration,
                         Animation::LoopOut looped,
                         Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new AScale(s, duration, looped, interpolationType));
}

void Animatable::a_color(const std::string &listName, const sf::Color &color, double duration, Animation::LoopOut looped,
                    Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new AColor(color, duration, looped, interpolationType));
}

void Animatable::a_wait(const std::string& listName, double duration) {
    animations[listName].emplace_back(new AWait(duration));
}

void Animatable::a_function(const std::string &listName,
                            std::function<void()> function,
                            int calls,
                            double duration,
                            Animation::LoopOut looped,
                            Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new AFunction(std::move(function), calls, duration, looped, interpolationType));
}
*/

void Animatable::update_animations() {

    for (auto& [listName, animationList] : animations) {

        if (animationList.empty())
            continue;
        auto it = animationList.begin();
        // If it the front animation is 'a_wait()' we should wait until waiting time is over

        if (it.operator*()->waitFor()) {
            if (!it.operator*()->update())
                animationList.erase(it);
            continue;
        }

        // Otherwise we iterate over all animation until we meet animations.end() or wait animation
        while (!animationList.empty() && (it != animationList.end()) && (!it.operator*()->waitFor())) {
            if (!it.operator*()->update())
                animationList.erase(it++);
            else
                it++;
        }
    }
}

void Animatable::animate(const std::string &listName, Animation* anim) {
    animations[listName].emplace_back(anim);
}
