#include "Barbarian.h"
#include <cstdlib>
#include <iostream>

Barbarian::Barbarian(const std::string& name, Race race, int maxHp, int strength, int magicka, int dexterity) {
    this->setName(name);
    this->setRace(race);
    this->setMaxHp(maxHp);
    this->setHp(maxHp);
    this->setStrength(strength);
    this->setDexterity(dexterity);
    this->setMagicka(magicka);
    this->setCarryWeight(strength * 30);
    this->setTurnPoints(3);
    this->setHealingPotions(2);
    this->setBaseDef(dexterity / 2);
}

void Barbarian::StartRage() {
    addStatusEffect(StatusEffect(EffectType::RAGE, 3, 0));
    setTurnPoints(getTurnPoints() - 1);
    std::cout << getName() << " enters RAGE! (3 turns)\n";
}

void Barbarian::Attack(Character& enemy) {
    int baseDamage = 0;
    bool isRaging = hasStatusEffect(EffectType::RAGE);

    if (isRaging) {
        int damage1 = std::rand() % getStrength() + 1;
        int damage2 = std::rand() % getStrength() + 1;
        if (damage1 > damage2) {
            baseDamage = damage1;
        }
        else {
            baseDamage = damage2;
        }
        std::cout << getName() << " uses Rage attack! ";
    }
    else {
        baseDamage = std::rand() % getStrength() + 1;
        std::cout << getName() << " attacks! ";
    }

    DamageInstance damage = createAttackDamage(baseDamage);
    int actualDamage = enemy.takeDamage(damage);

    if (actualDamage > 0) {
        std::cout << "Total damage: " << actualDamage << " HP\n";
    }
    else {
        std::cout << "Miss!\n";
    }

    setTurnPoints(getTurnPoints() - 2);
}

void Barbarian::Swing(std::vector<Character*>& enemies) {
    std::cout << getName() << " swings at all enemies!\n";
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i]->getHp() > 0) {
            int baseDamage = (std::rand() % getStrength() + 1) + getStrength() / 2;
            DamageInstance damage = createAttackDamage(baseDamage);
            int actualDamage = enemies[i]->takeDamage(damage);
            std::cout << "  Hit " << enemies[i]->getName() << " for " << actualDamage << " total damage!\n";
        }
    }
    setTurnPoints(getTurnPoints() - 3);
}