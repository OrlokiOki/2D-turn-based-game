#pragma once
#include "DamageTypes.h"
#include <map>

class Resistance {
private:
    std::map<DamageType, float> resistances; 

public:
    Resistance();

    void setResistance(DamageType type, float value);
    float getResistance(DamageType type) const;
    void addResistance(DamageType type, float amount); 
};