//
// Created by Neirokan on 09.05.2020
//

#include "ResourceManager.h"
#include "utils/Log.h"
#include <map>
#include <memory>
#include <sstream>
#include <fstream>

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
    unloadObjects();
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

    // If success - remember and return sound pointer
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

    // If success - remember and return font pointer
    _instance->_fonts.emplace(filename, font);

    return font;
}

std::vector<std::shared_ptr<Mesh>> ResourceManager::loadObjects(const std::string &filename, const Vec3D& scale) {

    // If objects is already loaded - return pointer to it
    auto it = _instance->_objects.find(filename);
    if (it != _instance->_objects.end())
        return it->second;


    std::vector<std::shared_ptr<Mesh>> objects{};
    std::map<std::string, sf::Color> maters{};

    std::ifstream file(filename);
    if (!file.is_open())
    {
        Log::log("Mesh::LoadObjects(): cannot load file from " + filename);
        return objects;
    }

    std::vector<Point4D> verts{};
    std::vector<Triangle> tris{};
    sf::Color currentColor = sf::Color(255, 245, 194, 255);

    while (!file.eof())
    {
        char line[128];
        file.getline(line, 128);

        std::stringstream s;
        s << line;

        char junk;
        if(line[0] == 'o') {
            if(!tris.empty()) {
                objects.push_back(make_shared<Mesh>(tris));
                objects.back()->scale(scale);
            }
            tris.clear();
        }
        if (line[0] == 'v')
        {
            double x, y, z;
            s >> junk >> x >> y >> z;
            verts.emplace_back(x, y, z, 1);
        }
        if(line[0] == 'g') {
            std::string matInfo;
            s >> junk >> matInfo;
            std::string colorName = matInfo.substr(matInfo.size()-3, 3);
            currentColor = maters[matInfo.substr(matInfo.size()-3, 3)];
        }
        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            tris.emplace_back(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1], currentColor);
        }
        if(line[0] == 'm')
        {
            int color[4];
            std::string matName;

            s >> junk >> matName >> color[0] >> color[1] >> color[2] >> color[3];
            maters.insert({matName, sf::Color(color[0],color[1],color[2], color[3])});
        }
    }

    if(!tris.empty()) {
        objects.push_back(make_shared<Mesh>(tris));
        objects.back()->scale(scale);
    }

    file.close();

    // If success - remember and return vector of objects pointer
    _instance->_objects.emplace(filename, objects);

    return objects;
}

void ResourceManager::unloadObjects() {
    _instance->_objects.clear();
}

void ResourceManager::free() {
    unloadAllResources();
    delete _instance;
    _instance = nullptr;
}
