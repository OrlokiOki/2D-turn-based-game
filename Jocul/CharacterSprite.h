#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "SpriteManager.h"
#include <vector>

class CharacterSprite {
private:
	Character* character;
	sf::Sprite* sprite;
	sf::Texture* currentTexture;
	AnimationState currentState;
	sf::Vector2f position;
	float animationTimer;
	bool isPlayer;
	sf::Text* hpText;
	sf::Text* nameText;
	sf::Text* manaText;
	std::vector<sf::Text*> statusTexts; 
	std::string spritePrefix;

public:
	CharacterSprite(Character* ch, bool player);
	void setTexture(sf::Texture* texture);
	void setState(AnimationState state);
	void setPosition(float x, float y);
	void updateAnimation(float deltaTime);
	void updateHPText(const sf::Font& font);
	void updateNameText(const sf::Font& font);
	void updateManaText(const sf::Font& font);
	void updateStatusText(const sf::Font& font);
	void setSpritePrefix(const std::string& prefix);

	AnimationState getState() const { return currentState; }
	sf::Sprite* getSpritePtr() { return sprite; }
	sf::Sprite& getSprite() { return *sprite; }
	sf::Text* getHPText() { return hpText; }
	sf::Text* getNameText() { return nameText; }
	sf::Text* getManaText() { return manaText; }
	std::vector<sf::Text*>& getStatusTexts() { return statusTexts; }
	Character* getCharacter() { return character; }
	bool getIsPlayer() const { return isPlayer; }
	bool hasSprite() const { return sprite != NULL; }
	bool containsPoint(float x, float y) const;
	std::string getSpritePrefix() const { return spritePrefix; }

	void playAttackAnimation();
	void playDamageAnimation(DamageType damageType);
	void playDeathAnimation();
	void returnToIdle();
};