//
// Created by Иван Ильин on 03.10.2021.
//

#ifndef SHOOTER_TIMELINE_H
#define SHOOTER_TIMELINE_H

#include "Animation.h"

class AnimationListTag final {
private:
    const std::string _name;
public:
    explicit AnimationListTag(std::string name = "") : _name(std::move(name)) {}
    [[nodiscard]] std::string str() const { return _name; }

    bool operator==(const AnimationListTag& tag) const { return _name == tag._name; }
    bool operator!=(const AnimationListTag& tag) const { return _name != tag._name; }
    bool operator<(const AnimationListTag& tag) const { return _name < tag._name; }
};

namespace Timeline {
    void update();
    void animate(const AnimationListTag& listName, Animation* anim);

    void deleteAllAnimations();
    void deleteAnimationList(const AnimationListTag& listName);

    [[nodiscard]] bool isInAnimList(const AnimationListTag& listName);
}

#endif //SHOOTER_TIMELINE_H
