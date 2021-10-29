//
// Created by Иван Ильин on 17.10.2021.
//

#include "SoundController.h"
#include "ResourceManager.h"

SoundController* SoundController::_instance = nullptr;
bool SoundController::_validInstance = false;


void SoundController::init() {
    _instance = new SoundController();
    _validInstance = true;
}

void SoundController::playSound(const SoundTag& soundTag, const std::string& filename) {
    if(!_validInstance) {
        return;
    }

    stopSound(soundTag);
    _instance->_sounds.emplace(soundTag, sf::Sound(*ResourceManager::loadSoundBuffer(filename)));
    _instance->_sounds[soundTag].play();
}

void SoundController::pauseSound(const SoundTag& soundTag) {
    if(!_validInstance) {
        return;
    }

    if(_instance->_sounds.count(soundTag) > 0) {
        _instance->_sounds[soundTag].pause();
    }
}

void SoundController::stopSound(const SoundTag& soundTag) {
    if(!_validInstance) {
        return;
    }

    if(_instance->_sounds.count(soundTag) > 0) {
        _instance->_sounds[soundTag].stop();
    }
    _instance->_sounds.erase(soundTag);
}

sf::Sound::Status SoundController::getStatus(const SoundTag& soundTag) {
    if(!_validInstance) {
        return sf::Sound::Status::Stopped;
    }

    if(_instance->_sounds.count(soundTag) > 0) {
        return _instance->_sounds[soundTag].getStatus();
    } else {
        _instance->_sounds.erase(soundTag);
        return sf::Sound::Status::Stopped;
    }
}

void SoundController::free() {
    if(_validInstance) {
        for(auto& [soundTag, sound] : _instance->_sounds) {
            sound.stop();
        }
    }

    _instance->_sounds.clear();
    _validInstance = false;
    delete _instance;
}
