#include "Enchantment.h"

Enchantment::Enchantment()
    : enchantType(EnchantmentType::NONE), damageType(DamageType::BLUDGEONING),
    bonusDamage(0), resistBonus(0.0f), name("None") {
}

Enchantment::Enchantment(std::string n, DamageType dmgType, int bonus)
    : enchantType(EnchantmentType::WEAPON_DAMAGE), damageType(dmgType),
    bonusDamage(bonus), resistBonus(0.0f), name(n) {
}

Enchantment::Enchantment(std::string n, DamageType dmgType, float resist)
    : enchantType(EnchantmentType::ARMOR_RESISTANCE), damageType(dmgType),
    bonusDamage(0), resistBonus(resist), name(n) {
}