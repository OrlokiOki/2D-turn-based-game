#pragma once
#include "DamageTypes.h"
#include <vector>

struct DamageComponent {
    DamageType type;
    int amount;

    DamageComponent(DamageType t, int amt) : type(t), amount(amt) {}
};

class DamageInstance {
private:
    std::vector<DamageComponent> damages;

public:
    void addDamage(DamageType type, int amount);
    const std::vector<DamageComponent>& getDamages() const { return damages; }
    int getTotalDamage() const;
};

