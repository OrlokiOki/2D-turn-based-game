#include "SpriteManager.h"
#include <iostream>

bool SpriteManager::loadTexture(const std::string& name, const std::string& filepath) {
    sf::Texture texture;
    if (!texture.loadFromFile(filepath)) {
        std::cout << "Failed to load texture: " << filepath << std::endl;
        return false;
    }
    textures[name] = texture;
    std::cout << "Loaded texture: " << name << std::endl;
    return true;
}

sf::Texture* SpriteManager::getTexture(const std::string& name) {
    if (textures.find(name) != textures.end()) {
        return &textures[name];
    }
    return NULL;
}

void SpriteManager::clear() {
    textures.clear();
}