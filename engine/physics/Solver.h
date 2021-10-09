//
// Created by Иван Ильин on 10.03.2021.
//

#ifndef ENGINE_SOLVER_H
#define ENGINE_SOLVER_H

#include "RigidBody.h"

class Solver {
public:
    static void solveCollision(std::shared_ptr<RigidBody> obj1, std::shared_ptr<RigidBody> obj2, const CollisionPoint& collision);
};


#endif //INC_3DZAVR_SOLVER_H
