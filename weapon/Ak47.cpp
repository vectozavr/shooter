//
// Created by Иван Ильин on 02.06.2021.
//

#include "../engine/ResourceManager.h"
#include "Ak47.h"
#include "../ShooterConsts.h"

using namespace std;

Ak47::Ak47(const std::string& weaponName) : Weapon(100, 30, 3.0, 0.1, 300, 2.0, ShooterConsts::AK47_FIRE_SOUND, ShooterConsts::AK47_RELOAD_SOUND, weaponName, ShooterConsts::AK47_OBJ, Vec3D{3, 3, 3}, Vec3D{-2.2, 1.0, 1.3}, Vec3D{0, Consts::PI, 0}) {
}
