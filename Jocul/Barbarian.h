#pragma once
#include "Character.h"
#include <vector>

class Barbarian : public Character
{
public:
    Barbarian(const std::string& name, Race race, int maxHp, int strength, int magicka, int dexterity);
    void StartRage();
    void Attack(Character& enemy);
    void Swing(std::vector<Character*>& enemies);
};