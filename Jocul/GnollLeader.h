#pragma once
#include "Rogue.h"

class GnollLeader : public Rogue
{
private:
	bool hasUsedInvisPotion; 

public:
	GnollLeader(const std::string& name, int maxHp, int strength, int magicka, int dexterity);
	void UseInvisPotion();
	void BleedingAttack(Character& target);
	bool getHasUsedInvisPotion() const { return hasUsedInvisPotion; }
};