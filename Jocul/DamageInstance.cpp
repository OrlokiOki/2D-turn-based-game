#include "DamageInstance.h"

void DamageInstance::addDamage(DamageType type, int amount) {
    damages.push_back(DamageComponent(type, amount));
}

int DamageInstance::getTotalDamage() const {
    int total = 0;
    for (size_t i = 0; i < damages.size(); i++) {
        total += damages[i].amount;
    }
    return total;
}