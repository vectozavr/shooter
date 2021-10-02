//
// Created by Иван Ильин on 03.10.2021.
//

#include <list>
#include "Animation.h"

namespace Timeline {
    namespace {
        std::map<std::string, std::list<Animation*>> _animations;
    }

    void animate(const std::string& listName, Animation* anim) {
        _animations[listName].emplace_back(anim);
    }

    void deleteAllAnimations() {
        _animations.clear();
    }

    void deleteAnimationList(const std::string& listName) {
        _animations[listName].clear();
        _animations.erase(listName);
    }

    [[nodiscard]] bool isInAnimList(const std::string& name) {
        return !_animations[name].empty();
    }

    void update() {
        for (auto& [listName, animationList] : _animations) {

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
}
