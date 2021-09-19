//
// Created by Иван Ильин on 02.06.2021.
//

#include <ResourceManager.h>
#include "Shotgun.h"

using namespace std;

Shotgun::Shotgun(int ammo, const std::string& weaponName) : Weapon(weaponName, "../obj/shotgun.obj", "../obj/shotgun_mat.txt", Point4D{3, 3, 3}, Point4D{-0.95, 1.3, -0.6}, Point4D{0, M_PI, 0}) {
    fireSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/weapons/shotgun.ogg"));
    reloadSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/weapons/reload_shotgun.ogg"));

    //reloadSound.setVolume(30);
    _initialPack = 15;
    _clipCapacity = 1; // how much ammo can be stored in one clipx
    _stockAmmo = ammo - _clipCapacity; // how much ammo do you have in stock
    _clipAmmo = _clipCapacity; // how much ammo do you have in current clip
    _reloadTime = 1;
    _fireDelay = 1; // time delay between fires
    _damage = 400;
    _spreading = 5;
}

std::map<std::string, double>
Shotgun::processFire(std::shared_ptr<World> world, std::shared_ptr<Camera> camera) {
    std::map<std::string, double> damagedPlayers;

    for(int i = 0; i < 15; i++) {
        //generate random vector
        Point4D randV(10*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX));

        // damage player
        auto rayCast = world->rayCast(camera->position(), camera->position() + camera->lookAt() * 1000 + randV);
        if (rayCast.second.find("Player") != std::string::npos) {
            damagedPlayers[rayCast.second] += _damage / (1.0 + (camera->position() - rayCast.first).abs());
        }

        // add trace line
        Point4D to = rayCast.first.w() == -1 ? camera->position() + camera->lookAt() * 1000 + randV: rayCast.first;
        string traceName = _name + "_trace_nr_" + std::to_string(fireTraces++);
        Point4D from = _objects[_name + "_" + to_string(9)]->position() +
                       _objects[_name + "_" + to_string(9)]->triangles()[0][0];
        world->addBody(make_shared<RigidBody>(Mesh::LineTo(from, to, 0.05)), traceName);
        world->body(traceName)->setCollider(false);

        // remove trace line after some time
        world->body(traceName)->a_color("color_trace", {255, 255, 255, 0}, 1, Animation::None, Animation::linear);
        world->body("map_0")->a_function(traceName + "delete", [world, traceName]() { deleteTrace(world, traceName); },
                                         1, 2);
        addTraceCallBack(from, to);
    }


    return damagedPlayers;
}