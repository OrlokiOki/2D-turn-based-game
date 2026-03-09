#pragma once
#include "Character.h"
#include <vector>

class Wizard : public Character
{
private:
    int manaPool;
public:
    Wizard(const std::string& name, Race race, int maxHp, int strength, int magicka, int dexterity);

    void setManaPool(int mana);
    int getManaPool();

    void Attack(Character& enemy);
    void Blizzard(std::vector<Character*>& enemies);
    void MagicMissile(Character& enemy);
};