//
// Created by Иван Ильин on 06.06.2021.
//

#include "../engine/ResourceManager.h"
#include "Rifle.h"
#include "../ShooterConsts.h"

Rifle::Rifle() : Weapon(5, 1, 1.0, 1.0, 30000, 0.5, ShooterConsts::RIFLE_FIRE_SOUND, ShooterConsts::RIFLE_RELOAD_SOUND, ObjectNameTag("rifle"), ShooterConsts::RIFLE_OBJ, Vec3D{3, 3, 3}, Vec3D{-2.3, 1, 1.3}, Vec3D{0, Consts::PI, 0}) {
}
