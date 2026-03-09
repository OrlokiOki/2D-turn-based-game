#include "Wizard.h"
#include <cstdlib>
#include <iostream>

Wizard::Wizard(const std::string& name, Race race, int maxHp, int strength, int magicka, int dexterity) : manaPool(magicka * 10) {
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

void Wizard::setManaPool(int mana) {
    manaPool = mana;
}

int Wizard::getManaPool() {
    return manaPool;
}

void Wizard::Attack(Character& enemy) {
    std::cout << getName() << " casts ice spike! ";

    DamageInstance damage;
    int magicDamage = (std::rand() % getMagicka() + 1) + getMagicka();
    damage.addDamage(DamageType::COLD, magicDamage);

    int actualDamage = enemy.takeDamage(damage);

    if (actualDamage > 0) {
        std::cout << "Total damage: " << actualDamage << " HP\n";
    }
    else {
        std::cout << "Miss!\n";
    }
    setTurnPoints(getTurnPoints() - 2);
}

void Wizard::Blizzard(std::vector<Character*>& enemies) {
    if (manaPool < 20) {
        std::cout << "Not enough mana!" << std::endl;
        return;
    }
    std::cout << getName() << " casts Blizzard!\n";
    for (size_t i = 0; i < enemies.size(); i++) {
        DamageInstance damage;
        int coldDamage = (std::rand() % (getMagicka() * 2) + getMagicka());
        damage.addDamage(DamageType::COLD, coldDamage);

        int actualDamage = enemies[i]->takeDamage(damage);
        std::cout << " Hit " << enemies[i]->getName() << " for " << actualDamage << " cold damage!\n";
    }
    manaPool -= 20;
    setTurnPoints(getTurnPoints() - 3);
}

void Wizard::MagicMissile(Character& enemy) {
    if (manaPool < 10) {
        std::cout << "Not enough mana!" << std::endl;
        return;
    }
    std::cout << getName() << " casts Magic Missile! ";

    DamageInstance damage;
    int forceDamage = getMagicka() * 2;
    damage.addDamage(DamageType::FORCE, forceDamage);

    if (enemy.hasStatusEffect(EffectType::INVISIBLE)) {
        std::cout << "\n " << enemy.getName() << "'s invisibility fades!\n";
        enemy.removeStatusEffect(EffectType::INVISIBLE);
        enemy.setBaseDef(enemy.getBaseDef() - 3);
    }

    enemy.setHp(enemy.getHp() - forceDamage);
    std::cout << "Guaranteed hit for " << forceDamage << " damage!\n";

    manaPool -= 10;
    setTurnPoints(getTurnPoints() - 2);
}
