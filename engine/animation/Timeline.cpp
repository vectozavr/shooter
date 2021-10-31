//
// Created by Иван Ильин on 03.10.2021.
//

#include <list>

#include "Animation.h"
#include "Timeline.h"
#include "../utils/Log.h"

Timeline *Timeline::_instance = nullptr;
bool Timeline::_validInstance = false;

void Timeline::init() {
    _instance = new Timeline();
    _validInstance = true;

    Log::log("Timeline::init(): animation timeline was initialized");
}

void Timeline::animate(const AnimationListTag &listName, std::shared_ptr<Animation> anim) {
    if (!_validInstance) {
        return;
    }

    _instance->_animations[listName].emplace_back(anim);

    Log::log("Timeline::animate(): add animation in '" + listName.str() + "' list");
}

void Timeline::deleteAllAnimations() {
    if (!_validInstance) {
        return;
    }

    int animCounter = 0;

    for (auto&[listName, animationList] : _instance->_animations) {
        animCounter += animationList.size();
        animationList.clear();
    }
    _instance->_animations.clear();

    Log::log("Timeline::deleteAllAnimations(): all " + std::to_string(animCounter) + " animations was deleted");
}

void Timeline::deleteAnimationList(const AnimationListTag &listName) {
    if (!_validInstance) {
        return;
    }

    int animCounter = _instance->_animations[listName].size();
    _instance->_animations[listName].clear();
    _instance->_animations.erase(listName);

    Log::log("Timeline::deleteAnimationList(): list '" + listName.str() + "' with " + std::to_string(animCounter) +
             " animations was deleted");
}

[[nodiscard]] bool Timeline::isInAnimList(const AnimationListTag &listName) {
    if (!_validInstance) {
        return false;
    }

    return !_instance->_animations[listName].empty();
}

void Timeline::update() {
    if (!_validInstance) {
        return;
    }

    for (auto&[listName, animationList] : _instance->_animations) {
        if (animationList.empty()) {
            _instance->_animations.erase(listName);
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

    Log::log("Timeline::free(): pointer to 'Timeline' was freed");
}
