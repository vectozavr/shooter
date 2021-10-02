//
// Created by Иван Ильин on 03.10.2021.
//

#ifndef SHOOTER_TIMELINE_H
#define SHOOTER_TIMELINE_H

#include "Animation.h"

namespace Timeline {
    void update();
    void animate(const std::string& listName, Animation* anim);

    void deleteAllAnimations();
    void deleteAnimationList(const std::string& listName);

    [[nodiscard]] bool isInAnimList(const std::string& name);
}

#endif //SHOOTER_TIMELINE_H
