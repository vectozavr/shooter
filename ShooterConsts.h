//
// Created by Иван Ильин on 10.10.2021.
//

#ifndef SHOOTER_SHOOTERCONSTS_H
#define SHOOTER_SHOOTERCONSTS_H

namespace ShooterConsts {
    const double GRAVITY = 35;
    const double HEALTH_MAX = 100;
    const double ABILITY_MAX = 10;
    const double JUMP_HEIGHT = 3;
    const double WALK_SPEED = 10;
    const double MOUSE_SENSITIVITY = 1.0 / 1000.0;
    const double SLOW_MO_COEFFICIENT = 5;
    const double FIRE_DISTANCE = 1000;
    const double BONUS_RECHARGE_TIME = 30;

    const std::string PLAYER_NAME = "Player";
    const std::string PROJECT_NAME = "Shooter";

    const std::string ABILITY_OBJ = "obj/ability.obj";
    const std::string HILL_OBJ = "obj/hill.obj";
    const std::string GUN_OBJ = "obj/gun.obj";
    const std::string GUN_FIRE_SOUND = "sound/weapons/gun.ogg";
    const std::string GUN_RELOAD_SOUND = "sound/weapons/reload_gun.ogg";
    const std::string AK47_OBJ = "obj/ak47.obj";
    const std::string AK47_FIRE_SOUND = "sound/weapons/ak47.ogg";
    const std::string AK47_RELOAD_SOUND = "sound/weapons/reload_ak47.ogg";
    const std::string GOLD_AK47_OBJ = "obj/gold_ak47.obj";
    const std::string GOLD_AK47_FIRE_SOUND = "sound/weapons/ak47.ogg";
    const std::string GOLD_AK47_RELOAD_SOUND = "sound/weapons/reload_ak47.ogg";
    const std::string RIFLE_OBJ = "obj/rifle.obj";
    const std::string RIFLE_FIRE_SOUND = "sound/weapons/shotgun.ogg";
    const std::string RIFLE_RELOAD_SOUND = "sound/weapons/reload_ak47.ogg";
    const std::string SHOTGUN_OBJ = "obj/shotgun.obj";
    const std::string SHOTGUN_FIRE_SOUND = "sound/weapons/shotgun.ogg";
    const std::string SHOTGUN_RELOAD_SOUND = "sound/weapons/reload_shotgun.ogg";

    const std::string CUBE_OBJ = "obj/cube.obj";
    const std::string MAP_OBJ = "maps/map1.obj";
    const std::string MAIN_MENU_BACK = "textures/back.png";
    const std::string MAIN_MENU_GUI = "textures/gui.png";

    const std::string HEAD_OBJ = "obj/man/head.obj";
    const std::string BODY_OBJ = "obj/man/body.obj";
    const std::string FOOT_OBJ = "obj/man/foot.obj";
    const std::string MAN_OBJ = "obj/man/man.obj";

    const std::string CLICK_SOUND = "sound/click.ogg";
    const std::string BACK_NOISE = "sound/backNoise.ogg";
    const std::string CHANGE_WEAPON_SOUND = "sound/weapons/change_weapon.ogg";
    const std::string RESTORE_HEALTH_SOUND = "sound/fullHealth.ogg";
    const std::string RESTORE_ABILITY_SOUND = "sound/fullAbility.ogg";
    const std::string KILL_SOUND = "sound/kill.ogg";
    const std::string DEATH_SOUND = "sound/classic_hurt.ogg";
    const std::string SLOW_MO_SOUND = "sound/slow_mo.ogg";
    const std::string UN_SLOW_MO_SOUND = "sound/unslow_mo.ogg";
    const std::string NO_AMMO_SOUND = "sound/weapons/no_ammo.ogg";

    const sf::Color WHITE_COLORS[] = {
            sf::Color(137, 135, 222), // blue
            sf::Color(195, 155, 209), // pink
            sf::Color(201, 137, 137), // red
            sf::Color(116, 204, 135), // green
            sf::Color(201, 171, 137), // orange
    };

    const sf::Color DARK_COLORS[] = {
            sf::Color(16, 18, 69), // blue
            sf::Color(77, 0, 62), // pink
            sf::Color(99, 20, 20), // red
            sf::Color(12, 46, 9), // green
            sf::Color(97, 70, 51), // orange
    };
}

#endif //SHOOTER_SHOOTERCONSTS_H
