//
// Created by Иван Ильин on 03.10.2021.
//

#include <list>
#include "Animation.h"
#include "Timeline.h"

Timeline* Timeline::_instance = nullptr;
bool Timeline::_validInstance = false;

void Timeline::init() {
    _instance = new Timeline();
    _validInstance = true;
}

void Timeline::animate(const AnimationListTag& listName, Animation* anim) {
    if(!_validInstance) {
        return;
    }

    _instance->_animations[listName].emplace_back(anim);
}

void Timeline::deleteAllAnimations() {
    if(!_validInstance) {
        return;
    }

    for (auto& [listName, animationList] : _instance->_animations) {
        auto it = animationList.begin();
        while(it != animationList.end()) {
            delete *(it++);
        }
        animationList.clear();
    }
    _instance->_animations.clear();
}

void Timeline::deleteAnimationList(const AnimationListTag& listName) {
    if(!_validInstance) {
        return;
    }

    _instance->_animations[listName].clear();
    _instance->_animations.erase(listName);
}

[[nodiscard]] bool Timeline::isInAnimList(const AnimationListTag& listName) {
    if(!_validInstance) {
        return false;
    }

    return !_instance->_animations[listName].empty();
}

void Timeline::update() {
    if(!_validInstance) {
        return;
    }

    for (auto& [listName, animationList] : _instance->_animations) {

        if (animationList.empty()) {
            continue;
        }
        auto it = animationList.begin();
        // If it the front animation is 'a_wait()' we should wait until waiting time is over

        if (it.operator*()->waitFor()) {
            if (!it.operator*()->updateState()) {
                animationList.erase(it);
            }
            continue;
        }

        // Otherwise we iterate over all animation until we meet animations.end() or wait animation
        while (!animationList.empty() && (it != animationList.end()) && (!it.operator*()->waitFor())) {
            if (!it.operator*()->updateState()) {
                animationList.erase(it++);
            } else {
                it++;
            }
        }
    }
}

void Timeline::free() {
    Timeline::deleteAllAnimations();
    _validInstance = false;

    delete _instance;
}
