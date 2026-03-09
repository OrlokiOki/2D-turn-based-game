#pragma once
#include <iostream>
#include "DamageTypes.h"
#include "Enchantment.h"

class Weapon
{
private:
    std::string name;
    int damage;
    std::string type;
    int strReq;
    int dexReq;
    int magReq;
    int weight;
    DamageType damageType;
    Enchantment enchantment;

public:
    Weapon(std::string name, int dam, std::string t, int s, int d, int m, int w, DamageType dmgType);

    void setDamage(int dam);
    void setStrReq(int s);
    void setDexReq(int d);
    void setMagReq(int m);
    void setType(std::string t);
    void setWeight(int w);
    void setName(std::string name);
    void setDamageType(DamageType dt);
    void setEnchantment(const Enchantment& ench);

    int getDamage() const;
    std::string getType() const;
    int getStrReq() const;
    int getDexReq() const;
    int getMagReq() const;
    int getWeight() const;
    std::string getName() const;
    DamageType getDamageType() const;
    const Enchantment& getEnchantment() const;

    bool operator==(const Weapon& other) const;
    Weapon& operator=(const Weapon& other);
};