#include "Rogue.h"
#include <cstdlib>
#include <iostream>

Rogue::Rogue(const std::string& name, Race race, int maxHp, int strength, int magicka, int dexterity) : sneak(false) {
	this->setName(name);
	this->setRace(race);
	this->setMaxHp(maxHp);
	this->setHp(maxHp);
	this->setStrength(strength);
	this->setDexterity(dexterity);
	this->setMagicka(magicka);
	this->setCarryWeight(strength * 30);
	this->setTurnPoints(3);
	this->setHealingPotions(3);
	this->setBaseDef(dexterity / 2);
}

void Rogue::setSneak(bool sneak) {
	this->sneak = sneak;
}

bool Rogue::getSneak() {
	return sneak;
}

void Rogue::Hide() {
	sneak = true;
	setTurnPoints(getTurnPoints() - 1);
	std::cout << getName() << " hides in the shadows!\n";
}

void Rogue::Attack(Character& enemy) {
	int baseDamage = 0;
	if (sneak == true) {
		baseDamage = (std::rand() % getDexterity() + 1) * 2;
		std::cout << getName() << " uses Sneak attack! ";
		sneak = false;
	}
	else {
		baseDamage = std::rand() % getDexterity() + 1;
		std::cout << getName() << " attacks! ";
	}
	DamageInstance damage = createAttackDamage(baseDamage);
	int actualDamage = enemy.takeDamage(damage);
	if (actualDamage > 0) {
		std::cout << "Total damage: " << actualDamage << " HP\n";
	}
	else {
		std::cout << "Miss!\n";
	}
	setTurnPoints(getTurnPoints() - 2);
}

void Rogue::SmokePowder(std::vector<Character*>& enemies) {
	std::cout << getName() << " throws smoke powder at all enemies!\n";
	for (size_t i = 0; i < enemies.size(); i++) {
		if (enemies[i]->getHp() > 0) {
			int chance = std::rand() % 100;
			if (chance < 50) {
				enemies[i]->addStatusEffect(StatusEffect(EffectType::BLINDNESS, 2, 0));
				std::cout << " " << enemies[i]->getName() << " is blinded for 2 turns!\n";
			}
			else {
				std::cout << " " << enemies[i]->getName() << " avoided the smoke!\n";
			}
		}
	}
	setTurnPoints(getTurnPoints() - 3);
}