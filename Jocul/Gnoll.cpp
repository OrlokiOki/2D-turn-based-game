#include "Gnoll.h"
#include <cstdlib>
#include <iostream>

Gnoll::Gnoll(const std::string& name, int maxHp, int strength, int magicka, int dexterity) {
	this->setName(name);
	this->setRace(Race::GNOLL);
	this->setMaxHp(maxHp);
	this->setHp(maxHp);
	this->setStrength(strength);
	this->setDexterity(dexterity);
	this->setMagicka(magicka);
	this->setCarryWeight(strength * 30);
	this->setTurnPoints(3);
	this->setHealingPotions(1);
	this->setBaseDef(dexterity / 2);
}

void Gnoll::Attack(Character& enemy) {
	std::cout << getName() << " attacks! ";
	int baseDamage = std::rand() % getStrength() + 1;

	if (hasStatusEffect(EffectType::BLINDNESS)) {
		int damage2 = std::rand() % getStrength() + 1;
		baseDamage = (baseDamage < damage2) ? baseDamage : damage2;
		std::cout << "(blinded) ";
	}

	DamageInstance damage = createAttackDamage(baseDamage);
	int actualDamage = enemy.takeDamage(damage);
	if (actualDamage > 0) {
		std::cout << "Hit for " << actualDamage << " damage!\n";
	}
	else {
		std::cout << "Miss!\n";
	}
	setTurnPoints(getTurnPoints() - 2);
}