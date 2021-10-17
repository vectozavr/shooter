//
// Created by Neirokan on 09.05.2020
//

#include "ResourceManager.h"
#include <map>
#include <memory>

ResourceManager* ResourceManager::_instance = nullptr;

void ResourceManager::init() {
    _instance = new ResourceManager();
}

void ResourceManager::unloadTextures() {
    for (auto & _texture : _instance->_textures)
        _texture.second.reset();
    _instance->_textures.clear();
}

void ResourceManager::unloadSoundBuffers() {
    for (auto & _soundBuffer : _instance->_soundBuffers)
        _soundBuffer.second.reset();
    _instance->_soundBuffers.clear();
}

void ResourceManager::unloadFonts() {
    for (auto & _font : _instance->_fonts)
        _font.second.reset();
    _instance->_fonts.clear();
}

void ResourceManager::unloadAllResources() {
    unloadTextures();
    unloadSoundBuffers();
    unloadFonts();
}

std::shared_ptr<sf::Texture> ResourceManager::loadTexture(const std::string& filename) {
    if(!_instance)
        return nullptr;

    // If texture is already loaded - return pointer to it
    auto it = _instance->_textures.find(filename);
    if (it != _instance->_textures.end())
        return it->second;

    // Otherwise - try to load it. If failure - return zero
    std::shared_ptr<sf::Texture> texture(new sf::Texture);
    if (!texture->loadFromFile(filename))
        return nullptr;

    // If success - remember and return texture pointer
    texture->setRepeated(true);
    _instance->_textures.emplace(filename, texture);

    return texture;
}

std::shared_ptr<sf::SoundBuffer> ResourceManager::loadSoundBuffer(const std::string& filename) {
    if(!_instance)
        return nullptr;

    // If sound buffer is already loaded - return pointer to it
    auto it = _instance->_soundBuffers.find(filename);
    if (it != _instance->_soundBuffers.end())
        return it->second;

    // Otherwise - try to load it. If failure - return zero
    std::shared_ptr<sf::SoundBuffer> soundBuffer(new sf::SoundBuffer);
    if (!soundBuffer->loadFromFile(filename))
        return nullptr;

    // If success - remember and return texture pointer
    _instance->_soundBuffers.emplace(filename, soundBuffer);

    return soundBuffer;
}

std::shared_ptr<sf::Font> ResourceManager::loadFont(const std::string& filename) {
    if(!_instance)
        return nullptr;

    // If font is already loaded - return pointer to it
    auto it = _instance->_fonts.find(filename);
    if (it != _instance->_fonts.end())
        return it->second;

    // Otherwise - try to load it. If failure - return zero
    std::shared_ptr<sf::Font> font(new sf::Font);
    if (!font->loadFromFile(filename))
        return nullptr;

    // If success - remember and return texture pointer
    _instance->_fonts.emplace(filename, font);

    return font;
}

void ResourceManager::free() {
    unloadAllResources();
    delete _instance;
    _instance = nullptr;
}

