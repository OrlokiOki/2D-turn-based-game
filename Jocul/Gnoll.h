#pragma once
#include "Character.h"

class Gnoll : public Character
{
public:
    Gnoll(const std::string& name, int maxHp, int strength, int magicka, int dexterity);
    void Attack(Character& enemy);
};