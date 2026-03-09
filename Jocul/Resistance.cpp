#include "Resistance.h"

Resistance::Resistance() {
    resistances[DamageType::BLUDGEONING] = 1.0f;
    resistances[DamageType::SLASHING] = 1.0f;
    resistances[DamageType::PIERCING] = 1.0f;
    resistances[DamageType::FIRE] = 1.0f;
    resistances[DamageType::COLD] = 1.0f;
    resistances[DamageType::LIGHTNING] = 1.0f;
    resistances[DamageType::POISON] = 1.0f;
    resistances[DamageType::NECROTIC] = 1.0f;
    resistances[DamageType::RADIANT] = 1.0f;
    resistances[DamageType::FORCE] = 1.0f;
}

void Resistance::setResistance(DamageType type, float value) {
    resistances[type] = value;
}

float Resistance::getResistance(DamageType type) const {
    std::map<DamageType, float>::const_iterator it = resistances.find(type);
    if (it != resistances.end()) {
        return it->second;
    }
    return 1.0f;
}

void Resistance::addResistance(DamageType type, float amount) {
    float current = getResistance(type);
    float newValue = current - amount; 
    if (newValue < 0.0f) newValue = 0.0f; 
    resistances[type] = newValue;
}