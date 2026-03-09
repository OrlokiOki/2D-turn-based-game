#include "GnollLeader.h"
#include <iostream>

GnollLeader::GnollLeader(const std::string& name, int maxHp, int strength, int magicka, int dexterity)
	: Rogue(name, Race::GNOLL, maxHp, strength, magicka, dexterity), hasUsedInvisPotion(false) {
}

void GnollLeader::UseInvisPotion() {
	if (hasUsedInvisPotion) {
		std::cout << getName() << " has already used their invisibility potion!\n";
		return;
	}

	setSneak(true);
	addStatusEffect(StatusEffect(EffectType::INVISIBLE, 999, 0));
	setBaseDef(getBaseDef() + 3);
	setTurnPoints(getTurnPoints() - 1);
	hasUsedInvisPotion = true; 
	std::cout << getName() << " drinks invisibility potion!\n";
}

void GnollLeader::BleedingAttack(Character& target) {
	std::cout << getName() << " uses Bleeding Attack! ";
	int baseDamage = std::rand() % getStrength() + 1;
	DamageInstance damage = createAttackDamage(baseDamage);
	int actualDamage = target.takeDamage(damage);
	if (actualDamage > 0) {
		std::cout << "Hit for " << actualDamage << " damage! Target is bleeding for 2 turns!\n";
		target.addStatusEffect(StatusEffect(EffectType::BLEEDING, 2, 3));
	}
	else {
		std::cout << "Miss!\n";
	}
	setTurnPoints(getTurnPoints() - 2);
}
