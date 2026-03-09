#pragma once
#include "DamageTypes.h"
#include <string>

enum class EnchantmentType {
    NONE,
    WEAPON_DAMAGE,   
    ARMOR_RESISTANCE 
};

class Enchantment {
private:
    EnchantmentType enchantType;
    DamageType damageType;
    int bonusDamage;      
    float resistBonus;
    std::string name;

public:
    Enchantment();
    Enchantment(std::string n, DamageType dmgType, int bonus);
    Enchantment(std::string n, DamageType dmgType, float resist);

    EnchantmentType getEnchantmentType() const { return enchantType; }
    DamageType getDamageType() const { return damageType; }
    int getBonusDamage() const { return bonusDamage; }
    float getResistBonus() const { return resistBonus; }
    std::string getName() const { return name; }
    bool hasEnchantment() const { return enchantType != EnchantmentType::NONE; }
};