#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

enum class AnimationState {
    IDLE,
    ATTACK,
    DEAD,
    DAMAGE_SLASHING,   
    DAMAGE_COLD
};

class SpriteManager {
private:
    std::map<std::string, sf::Texture> textures;
public:
    bool loadTexture(const std::string& name, const std::string& filepath);
    sf::Texture* getTexture(const std::string& name);
    void clear();
};