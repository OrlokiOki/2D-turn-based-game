#pragma once
#include "Character.h"
#include <vector>

class Rogue : public Character
{
private:
	bool sneak;
public:
	Rogue(const std::string& name, Race race, int maxHp, int strength, int magicka, int dexterity);
	void Hide();
	void Attack(Character& enemy);
	void SmokePowder(std::vector<Character*>& enemies); 
	void setSneak(bool sneak);
	bool getSneak();
};