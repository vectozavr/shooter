//
// Created by Иван Ильин on 26.01.2021.
//

#ifndef ENGINE_ANIMATABLE_H
#define ENGINE_ANIMATABLE_H

#include <list>
#include "../Triangle.h"

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

    void animate(const std::string& listName, Animation* anim);

    void update_animations();

    void stopAllAnimations() { animations.clear(); }
    void stopAnimationList(const std::string& name) { animations[name].clear(); }

    [[nodiscard]] bool isInAnim() const {
        for(auto& animList : animations)
            if (!animList.second.empty())
                return true;
        return false;

    }
    [[nodiscard]] bool isInAnimList(const std::string& name) { return !animations[name].empty(); }
};

#endif //INC_3DZAVR_ANIMATABLE_H
