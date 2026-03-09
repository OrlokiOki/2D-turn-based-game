#pragma once
#include <string>

enum class EffectType {
    NONE,
    BLEEDING,
    INVISIBLE,
    RAGE,
    BURNING,
    FROZEN,
    POISONED,
    BLINDNESS
};

class StatusEffect {
private:
    EffectType type;
    int duration;
    int damagePerTurn;

public:
    StatusEffect(EffectType t, int dur, int dmg = 0);

    EffectType getType() const { return type; }
    int getDuration() const { return duration; }
    int getDamagePerTurn() const { return damagePerTurn; }

    void decrementDuration() { duration--; }
    bool isExpired() const { return duration <= 0; }
};
