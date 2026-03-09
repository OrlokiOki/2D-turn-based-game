#include "StatusEffect.h"

StatusEffect::StatusEffect(EffectType t, int dur, int dmg)
    : type(t), duration(dur), damagePerTurn(dmg) {
}