//
// Created by Иван Ильин on 01.06.2021.
//

#include <sstream>
#include "Weapon.h"
#include "ResourceManager.h"
#include "utils/Log.h"

using namespace std;

Weapon::Weapon(const std::string& weaponName, const std::string& objFileName, const std::string& matFileName, const Point4D& scale, const Point4D& translate, const Point4D& rotate) {
    _name = weaponName;
    auto objs = Mesh::LoadObjects(objFileName, matFileName, scale);
    for(int i = 0; i < objs.size(); i++) {
        string meshName = _name + "_" + to_string(i);
        objs[i]->setCollider(false);

        //transforms
        objs[i]->rotate(rotate);
        objs[i]->translate(translate);

        _objects.insert({meshName, objs[i]});
    }
    noAmmoSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/weapons/no_ammo.ogg"));
}

std::map<std::string, double> Weapon::fire(const std::shared_ptr<World>& world, const std::shared_ptr<Camera>& camera) {
    if(_clipAmmo == 0) {
        reload();
        if(_clipAmmo == 0)
            noAmmoSound.play();
    }

    if(_clipAmmo <= 0 || std::abs(Time::time() - _lastFireTime) < _fireDelay || std::abs(Time::time() - _lastReloadTime) < _reloadTime)
        return std::map<std::string, double>();

    _lastFireTime = Time::time();
    _clipAmmo--;

    fireSound.play();
    Log::log("Weapon::fire (" + std::to_string(_stockAmmo) + " : " + std::to_string(_clipAmmo) + ")");

    return processFire(world, camera);
}

void Weapon::reload() {
    if (_stockAmmo == 0 || std::abs(Time::time() - _lastReloadTime) < _reloadTime)
        return;
    if(_clipCapacity - _clipAmmo <= _stockAmmo) {
        _stockAmmo -= _clipCapacity - _clipAmmo;
        _clipAmmo = _clipCapacity;
    } else {
        _clipAmmo += _stockAmmo;
        _stockAmmo = 0;
    }

    reloadSound.play();
    Log::log("Weapon::reload (" + std::to_string(_stockAmmo) + " : " + std::to_string(_clipAmmo) + ")");
    _lastReloadTime = Time::time();
}

void Weapon::addToWorld(const shared_ptr<World> &world) {
    for(auto& obj : _objects) {
        world->addMesh(obj.second, obj.first);
    }
}

void Weapon::removeFromWorld(const shared_ptr<World> &world) {
    for(auto& obj : _objects) {
        world->removeMeshInstantly(obj.first);
    }
}


void Weapon::attachToPlayer(Mesh &player) {
    for(auto& obj : _objects) {
        player.attach(obj.second);
    }
}

void Weapon::rotate(const Point4D& point4D, double val) {
    for(auto& mesh : _objects)
        mesh.second->rotate(point4D, val);
}

void Weapon::translate(const Point4D &point4D) {
    for(auto& mesh : _objects)
        mesh.second->translate(point4D);
}

void Weapon::deleteTrace(const shared_ptr<World> &world, const std::string& traceName) {
    world->removeMesh(traceName);
}

void Weapon::rotateRelativePoint(const Point4D &point4D, const Point4D &v, double val) {
    for(auto& mesh : _objects)
        mesh.second->rotateRelativePoint(point4D, v, val);
}

std::map<std::string, double> Weapon::processFire(const shared_ptr<World> &world, const shared_ptr<Camera> &camera) {
    std::map<std::string, double> damagedPlayers;

    //generate random vector
    Point4D randV(10.0*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10.0*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10.0*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX));

    // damage player
    auto rayCast = world->rayCast(camera->position(), camera->position() + camera->lookAt() * 1000 + randV);
    if(rayCast.second.find("Player") != std::string::npos) {
        damagedPlayers[rayCast.second] += _damage/(1.0 + (camera->position() - rayCast.first).abs());
    }

    // add trace line
    Point4D to = rayCast.first.w() == -1 ? camera->position() + camera->lookAt() * 1000 + randV: rayCast.first;
    string traceName = _name + "_trace_nr_" + std::to_string(fireTraces++);
    Point4D from = _objects[_name + "_" + to_string(_objects.size()-1)]->position() + _objects[_name + "_" + to_string(_objects.size()-1)]->triangles()[0][0];
    world->addMesh(make_shared<Mesh>(Mesh::LineTo(from, to, 0.05)), traceName);
    (*world)[traceName]->setCollider(false);

    // remove trace line after some time
    (*world)[traceName]->a_color("color_trace", {255, 255, 255, 0}, 1, Animation::None, Animation::linear);
    (*world)["Player_im"]->a_function(traceName + "delete", [world, traceName](){deleteTrace(world, traceName); }, 1, 2);

    addTraceCallBack(from, to);

    return damagedPlayers;
}
