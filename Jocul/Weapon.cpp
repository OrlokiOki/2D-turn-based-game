#include "Weapon.h"

Weapon::Weapon(std::string name, int dam, std::string t, int s, int d, int m, int w, DamageType dmgType) {
    this->name = name;
    damage = dam;
    type = t;
    strReq = s;
    dexReq = d;
    magReq = m;
    weight = w;
    damageType = dmgType;
}

void Weapon::setDamage(int dam) {
    damage = dam;
}

void Weapon::setStrReq(int s) {
    strReq = s;
}

void Weapon::setDexReq(int d) {
    dexReq = d;
}

void Weapon::setMagReq(int m) {
    magReq = m;
}

void Weapon::setType(std::string t) {
    type = t;
}

int Weapon::getDamage() const {
    return damage;
}

std::string Weapon::getType() const {
    return type;
}

int Weapon::getStrReq() const {
    return strReq;
}

int Weapon::getDexReq() const {
    return dexReq;
}

int Weapon::getMagReq() const {
    return magReq;
}

void Weapon::setWeight(int w) {
    weight = w;
}

int Weapon::getWeight() const {
    return weight;
}

std::string Weapon::getName() const {
    return name;
}

void Weapon::setName(std::string name) {
    this->name = name;
}

DamageType Weapon::getDamageType() const {
    return damageType;
}

void Weapon::setDamageType(DamageType dt) {
    damageType = dt;
}

const Enchantment& Weapon::getEnchantment() const {
    return enchantment;
}

void Weapon::setEnchantment(const Enchantment& ench) {
    enchantment = ench;
}

bool Weapon::operator==(const Weapon& other) const {
    return (name == other.name &&
        damage == other.damage &&
        type == other.type &&
        strReq == other.strReq &&
        magReq == other.magReq &&
        dexReq == other.dexReq &&
        weight == other.weight);
}

Weapon& Weapon::operator=(const Weapon& other) {
    if (this != &other) {
        name = other.name;
        damage = other.damage;
        type = other.type;
        strReq = other.strReq;
        magReq = other.magReq;
        dexReq = other.dexReq;
        weight = other.weight;
        damageType = other.damageType;
        enchantment = other.enchantment;
    }
    return *this;
}