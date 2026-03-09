#include "CharacterSprite.h"
#include "Wizard.h"

CharacterSprite::CharacterSprite(Character* ch, bool player)
	: character(ch), sprite(NULL), currentTexture(NULL),
	currentState(AnimationState::IDLE),
	animationTimer(0.0f), isPlayer(player), hpText(NULL), nameText(NULL),
	manaText(NULL), spritePrefix("orc") {
}

void CharacterSprite::setSpritePrefix(const std::string& prefix) {
	spritePrefix = prefix;
}

void CharacterSprite::setTexture(sf::Texture* texture) {
	if (texture) {
		currentTexture = texture;
		if (!sprite) {
			sprite = new sf::Sprite(*texture);
			sprite->setScale({ 3.0f, 3.0f });
			sprite->setPosition(position);
		}
		else {
			sprite->setTexture(*texture);
			sf::Vector2u textureSize = texture->getSize();
			sprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(textureSize.x, textureSize.y)));
		}
	}
}

void CharacterSprite::setState(AnimationState state) {
	currentState = state;
	animationTimer = 0.0f;
}

void CharacterSprite::setPosition(float x, float y) {
	position.x = x;
	position.y = y;
	if (sprite) {
		sprite->setPosition(position);
	}
	if (hpText) {
		hpText->setPosition(sf::Vector2f({ x, y - 20.0f }));
	}
	if (nameText) {
		nameText->setPosition(sf::Vector2f(x + 50.0f, y - 20.0f));
	}
	if (manaText) {
		manaText->setPosition(sf::Vector2f(x, y - 5.0f));
	}
	for (size_t i = 0; i < statusTexts.size(); i++) {
		if (statusTexts[i]) {
			statusTexts[i]->setPosition(sf::Vector2f(x + 50.0f + (i * 20.0f), y + 10.0f));
		}
	}
}

void CharacterSprite::updateAnimation(float deltaTime) {
	animationTimer += deltaTime;
	//for attack
	if (currentState == AnimationState::ATTACK && animationTimer > 1.0f) {
		returnToIdle();
	}
	//for damage taken
	if ((currentState == AnimationState::DAMAGE_SLASHING ||currentState == AnimationState::DAMAGE_COLD) && animationTimer > 1.0f) {
		returnToIdle();
	}
	//for effects
	if (sprite && character->hasStatusEffect(EffectType::INVISIBLE)) {
		sprite->setColor(sf::Color(255, 255, 255, 100));
	}
	else if (sprite) {
		sprite->setColor(sf::Color(255, 255, 255, 255));
	}
}
void CharacterSprite::updateHPText(const sf::Font& font) {
	if (!hpText) {
		hpText = new sf::Text(font);
		hpText->setCharacterSize(16);
		hpText->setFillColor(sf::Color::White);
	}
	std::string hpString = std::to_string(character->getHp()) + "/" + std::to_string(character->getMaxHp());
	hpText->setString(hpString);
	hpText->setPosition(sf::Vector2f(position.x, position.y - 20.0f));
}

void CharacterSprite::updateNameText(const sf::Font& font) {
	if (!nameText) {
		nameText = new sf::Text(font);
		nameText->setCharacterSize(16);
		nameText->setFillColor(sf::Color::Yellow);
	}
	nameText->setString(character->getName());
	nameText->setPosition(sf::Vector2f(position.x + 50.0f, position.y - 20.0f));
}

void CharacterSprite::updateManaText(const sf::Font& font) {
	Wizard* wizard = static_cast<Wizard*>(character);
	if (wizard) {
		if (!manaText) {
			manaText = new sf::Text(font);
			manaText->setCharacterSize(14);
			manaText->setFillColor(sf::Color::Cyan);
		}
		std::string manaString = "Mana: " + std::to_string(wizard->getManaPool());
		manaText->setString(manaString);
		manaText->setPosition(sf::Vector2f(position.x, position.y - 5.0f));
	}
}

void CharacterSprite::updateStatusText(const sf::Font& font) {
	for (size_t i = 0; i < statusTexts.size(); i++) {
		delete statusTexts[i];
	}
	statusTexts.clear();

	std::vector<StatusEffect>& effects = character->getActiveEffects();

	for (size_t i = 0; i < effects.size(); i++) {
		sf::Text* text = new sf::Text(font);
		text->setCharacterSize(18);

		std::string letter = "";

		switch (effects[i].getType()) {
		case EffectType::BLEEDING:
			text->setFillColor(sf::Color::Red);
			letter = "B";
			break;
		case EffectType::INVISIBLE:
			text->setFillColor(sf::Color::White);
			letter = "I";
			break;
		case EffectType::BLINDNESS:
			text->setFillColor(sf::Color(128, 128, 128));
			letter = "Bl";
			break;
		case EffectType::RAGE:
			text->setFillColor(sf::Color::Red);
			letter = "R";
			break;
		default:
			delete text;
			continue;
		}

		text->setString(letter);
		text->setPosition(sf::Vector2f(position.x + 50.0f + (i * 20.0f), position.y + 10.0f));
		statusTexts.push_back(text);
	}
}

bool CharacterSprite::containsPoint(float x, float y) const {
	if (!sprite) return false;
	sf::FloatRect bounds = sprite->getGlobalBounds();
	return bounds.contains(sf::Vector2f(x, y));
}

void CharacterSprite::playAttackAnimation() {
	setState(AnimationState::ATTACK);
}

void CharacterSprite::playDamageAnimation(DamageType damageType) {
	if (damageType == DamageType::SLASHING ||damageType == DamageType::PIERCING ||damageType == DamageType::BLUDGEONING||damageType == DamageType::FORCE) {
		setState(AnimationState::DAMAGE_SLASHING);
	}
	else if (damageType == DamageType::COLD ||damageType == DamageType::FIRE ||damageType == DamageType::LIGHTNING) {
		setState(AnimationState::DAMAGE_COLD);
	}
}

void CharacterSprite::playDeathAnimation() {
	setState(AnimationState::DEAD);
}

void CharacterSprite::returnToIdle() {
	if (character->getHp() > 0) {
		setState(AnimationState::IDLE);
	}
	else {
		setState(AnimationState::DEAD);
	}
}