#include "Armor.h"

Armor::Armor(std::string name, int deff, std::string t, int s, int w) {
    this->name = name;
    def = deff;
    type = t;
    strReq = s;
    weight = w;
}

void Armor::setDef(int deff) {
    def = deff;
}

void Armor::setStrReq(int s) {
    strReq = s;
}

void Armor::setType(std::string t) {
    type = t;
}

int Armor::getDef() const {
    return def;
}

std::string Armor::getType() const {
    return type;
}

int Armor::getStrReq() const {
    return strReq;
}

void Armor::setWeight(int w) {
    weight = w;
}

int Armor::getWeight() const {
    return weight;
}

std::string Armor::getName() const {
    return name;
}

void Armor::setName(std::string name) {
    this->name = name;
}

void Armor::setEnchantment(const Enchantment& ench) {
    enchantment = ench;
}

const Enchantment& Armor::getEnchantment() const {
    return enchantment;
}

bool Armor::operator==(const Armor& other) const {
    return (name == other.name &&
        def == other.def &&
        type == other.type &&
        strReq == other.strReq &&
        weight == other.weight);
}

Armor& Armor::operator=(const Armor& other) {
    if (this != &other) {
        name = other.name;
        def = other.def;
        type = other.type;
        strReq = other.strReq;
        weight = other.weight;
        enchantment = other.enchantment;
    }
    return *this;
}