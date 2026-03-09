#pragma once
#include <iostream>
#include <vector>
#include "Armor.h"
#include "Weapon.h"
#include "DamageTypes.h"
#include "StatusEffect.h"
#include "Resistance.h"
#include "DamageInstance.h"

class Character
{
private:
    std::string name;
    int maxHp;
    int hp;
    int strength;
    int magicka;
    int dexterity;
    int carryWeight;
    int turnPoints;
    int baseDef;
    int healingPotions;
    Race race;
    Resistance resistances;
    std::vector<StatusEffect> activeEffects;
    DamageType lastDamageType;

public:
    std::vector<Armor> Armors;
    std::vector<Weapon> Weapons;
    Armor eqArmor;
    Weapon eqWeapon;

    Character();

    void setName(const std::string& n);
    void setMaxHp(int h);
    void setHp(int h);
    void setStrength(int s);
    void setMagicka(int m);
    void setDexterity(int d);
    void setCarryWeight(int c);
    void setTurnPoints(int turnP);
    void setBaseDef(int baseD);
    void setHealingPotions(int healP);
    void setRace(Race r);


    std::string getName() const;
    int getMaxHp() const;
    int getHp() const;
    int getStrength() const;
    int getMagicka() const;
    int getDexterity() const;
    int getCarryWeight() const;
    int getTurnPoints() const;
    int getBaseDef() const;
    int getHealingPotions() const;
    Race getRace() const;
    DamageType getLastDamageType() const { return lastDamageType; }

    Resistance& getResistances();
    std::vector<StatusEffect>& getActiveEffects();

    void addStatusEffect(const StatusEffect& effect);
    void removeStatusEffect(EffectType type);
    bool hasStatusEffect(EffectType type) const;
    void updateStatusEffects();
    void initializeRacialResistances();

    void unequipArmor();
    void equipArmor(Armor arm);
    void unequipWeapon();
    void equipWeapon(Weapon wep);

    int takeDamage(const DamageInstance& damage);
    DamageInstance createAttackDamage(int baseDamage);

    void Brace();
    void UseHealingPotion();
};