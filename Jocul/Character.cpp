#include "Character.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

Character::Character() : eqArmor("", 0, "No type", 0, 0),
eqWeapon("", 0, "No weapon", 0, 0, 0, 0, DamageType::BLUDGEONING),
race(Race::HUMAN), name("Unknown") {
}

void Character::setName(const std::string& n) {
    name = n;
}

void Character::setMaxHp(int hm) {
    maxHp = hm;
}

void Character::setHp(int h) {
    hp = h;
    if (hp > maxHp) hp = maxHp;
    if (hp < 0) hp = 0;
}

void Character::setStrength(int s) {
    strength = s;
}

void Character::setMagicka(int m) {
    magicka = m;
}

void Character::setDexterity(int d) {
    dexterity = d;
}

void Character::setCarryWeight(int c) {
    carryWeight = c;
}

void Character::setTurnPoints(int turnP) {
    turnPoints = turnP;
}

void Character::setBaseDef(int baseD) {
    baseDef = baseD;
}

void Character::setHealingPotions(int healingP) {
    healingPotions = healingP;
}

void Character::setRace(Race r) {
    race = r;
    initializeRacialResistances();
}

std::string Character::getName() const {
    return name;
}

int Character::getMaxHp() const {
    return maxHp;
}

int Character::getHp() const {
    return hp;
}

int Character::getStrength() const {
    return strength;
}

int Character::getMagicka() const {
    return magicka;
}

int Character::getDexterity() const {
    return dexterity;
}

int Character::getCarryWeight() const {
    return carryWeight;
}

int Character::getTurnPoints() const {
    return turnPoints;
}

int Character::getHealingPotions() const {
    return healingPotions;
}

int Character::getBaseDef() const {
    return baseDef;
}

Race Character::getRace() const {
    return race;
}

Resistance& Character::getResistances() {
    return resistances;
}

std::vector<StatusEffect>& Character::getActiveEffects() {
    return activeEffects;
}

void Character::addStatusEffect(const StatusEffect& effect) {
    for (size_t i = 0; i < activeEffects.size(); i++) {
        if (activeEffects[i].getType() == effect.getType()) {
            activeEffects[i] = effect;
            return;
        }
    }
    activeEffects.push_back(effect);
}

void Character::removeStatusEffect(EffectType type) {
    for (size_t i = 0; i < activeEffects.size(); i++) {
        if (activeEffects[i].getType() == type) {
            activeEffects.erase(activeEffects.begin() + i);
            return;
        }
    }
}

bool Character::hasStatusEffect(EffectType type) const {
    for (size_t i = 0; i < activeEffects.size(); i++) {
        if (activeEffects[i].getType() == type) {
            return true;
        }
    }
    return false;
}

void Character::updateStatusEffects() {
    for (size_t i = 0; i < activeEffects.size(); ) {
        StatusEffect& effect = activeEffects[i];

        if (effect.getDamagePerTurn() > 0) {
            setHp(getHp() - effect.getDamagePerTurn());
            std::cout << "  Taking " << effect.getDamagePerTurn() << " damage from status effect!\n";
        }

        effect.decrementDuration();

        if (effect.isExpired()) {
            std::cout << "  Status effect expired!\n";
            activeEffects.erase(activeEffects.begin() + i);
        }
        else {
            i++;
        }
    }
}

void Character::initializeRacialResistances() {
    if (race == Race::TIEFLING) {
        resistances.setResistance(DamageType::FIRE, 0.8f);
    }
}

int Character::takeDamage(const DamageInstance& damage) {
    int totalDamage = 0;
    const std::vector<DamageComponent>& damages = damage.getDamages();

    if (damages.size() > 0) {
        lastDamageType = damages[0].type;
    }

    if (eqArmor.getEnchantment().hasEnchantment() &&
        eqArmor.getEnchantment().getEnchantmentType() ==
        EnchantmentType::ARMOR_RESISTANCE) {
        DamageType resistType = eqArmor.getEnchantment().getDamageType();
        float resistBonus = eqArmor.getEnchantment().getResistBonus();
        resistances.addResistance(resistType, resistBonus);
    }

    for (size_t i = 0; i < damages.size(); i++) {
        float resistance = resistances.getResistance(damages[i].type);
        int actualDamage = (int)(damages[i].amount * resistance);
        totalDamage += actualDamage;
        std::cout << " Dealing " << actualDamage << " ";
        switch (damages[i].type) {
        case DamageType::BLUDGEONING: std::cout << "bludgeoning"; break;
        case DamageType::SLASHING: std::cout << "slashing"; break;
        case DamageType::PIERCING: std::cout << "piercing"; break;
        case DamageType::FIRE: std::cout << "fire"; break;
        case DamageType::COLD: std::cout << "cold"; break;
        case DamageType::LIGHTNING: std::cout << "lightning"; break;
        case DamageType::POISON: std::cout << "poison"; break;
        default: break;
        }
        std::cout << " damage\n";
    }

    int def = getBaseDef() + eqArmor.getDef();
    totalDamage -= def;
    if (totalDamage < 0) totalDamage = 0;

    if (totalDamage > 0 && hasStatusEffect(EffectType::INVISIBLE)) {
        std::cout << " " << getName() << "'s invisibility fades!\n";
        removeStatusEffect(EffectType::INVISIBLE);
        setBaseDef(getBaseDef() - 3);
    }

    setHp(getHp() - totalDamage);
    return totalDamage;
}

DamageInstance Character::createAttackDamage(int baseDamage) {
    DamageInstance damage;

    if (eqWeapon.getType() == "No weapon") {
        damage.addDamage(DamageType::BLUDGEONING, baseDamage);
    }
    else {
        damage.addDamage(eqWeapon.getDamageType(), baseDamage + eqWeapon.getDamage());

        if (eqWeapon.getEnchantment().hasEnchantment() && eqWeapon.getEnchantment().getEnchantmentType() == EnchantmentType::WEAPON_DAMAGE) {
            damage.addDamage(eqWeapon.getEnchantment().getDamageType(),
                eqWeapon.getEnchantment().getBonusDamage());
        }
    }

    return damage;
}

void Character::unequipArmor() {
    if (eqArmor.getType() == "No armor")
        std::cout << "Character has no armor on!";
    else {
        Armors.push_back(eqArmor);
        eqArmor.setName("");
        eqArmor.setDef(0);
        eqArmor.setStrReq(0);
        eqArmor.setType("No armor");
        eqArmor.setWeight(0);
    }
}

void Character::equipArmor(Armor arm) {
    if (eqArmor.getName() == arm.getName())
        std::cout << "You already have this armor on!";
    else if (strength >= arm.getStrReq()) {
        if (eqArmor.getName() == "") {
            eqArmor = arm;
            std::vector<Armor>::iterator it = std::find(Armors.begin(), Armors.end(), arm);
            if (it != Armors.end()) {
                Armors.erase(it);
            }
        }
        else {
            Armors.push_back(eqArmor);
            eqArmor = arm;
        }
    }
    else
        std::cout << "This armor is wayyy too heavy for you!";
}

void Character::unequipWeapon() {
    if (eqWeapon.getType() == "No weapon")
        std::cout << "Character has no weapon!";
    else {
        Weapons.push_back(eqWeapon);
        eqWeapon.setName("");
        eqWeapon.setDamage(0);
        eqWeapon.setStrReq(0);
        eqWeapon.setMagReq(0);
        eqWeapon.setDexReq(0);
        eqWeapon.setType("No weapon");
        eqWeapon.setWeight(0);
    }
}

void Character::equipWeapon(Weapon wep) {
    if (eqWeapon.getName() == wep.getName())
        std::cout << "You already have this weapon on!";
    else if (strength >= wep.getStrReq() && magicka >= wep.getMagReq() && dexterity >= wep.getDexReq()) {
        if (eqWeapon.getName() == "") {
            eqWeapon = wep;
            std::vector<Weapon>::iterator it = std::find(Weapons.begin(), Weapons.end(), wep);
            if (it != Weapons.end()) {
                Weapons.erase(it);
            }
        }
        else {
            Weapons.push_back(eqWeapon);
            eqWeapon = wep;
        }
    }
    else
        std::cout << "You cannot wield this weapon!";
}

void Character::Brace() {
    setBaseDef(getBaseDef() + 2);
    setTurnPoints(getTurnPoints() - 2);
}

void Character::UseHealingPotion() {
    int random = std::rand() % (20 - 10 + 1) + 10;
    if (random + getHp() < getMaxHp())
        setHp(random + getHp());
    else setHp(getMaxHp());
    setHealingPotions(getHealingPotions() - 1);
    setTurnPoints(getTurnPoints() - 1);
}