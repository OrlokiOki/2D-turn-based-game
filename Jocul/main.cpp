#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Barbarian.h"
#include "Rogue.h"
#include "Wizard.h"
#include "GnollLeader.h"
#include "Gnoll.h"
#include "CombatManager.h"
#include "CombatVisualizer.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));

    Weapon axe("Battle Axe", 3, "Axe", 3, 0, 0, 5, DamageType::SLASHING);
    Weapon knife("Dagger", 2, "Knife", 1, 2, 0, 1, DamageType::SLASHING);
    Weapon flail("Heavy Flail", 8, "Flail", 4, 0, 0, 6, DamageType::SLASHING);

    Barbarian barbarian("Yotul", Race::ORC, 40, 6, 2, 3);
    barbarian.equipWeapon(axe);  

    Rogue rogue("Vas", Race::DROW, 30, 3, 1, 6);
    rogue.equipWeapon(knife);  

    Wizard wizard("Clarity", Race::TIEFLING, 25, 2, 7, 4);

    GnollLeader gnollLeader("Gnoll Leader", 45, 5, 1, 4);
    gnollLeader.setHealingPotions(1);  
    gnollLeader.equipWeapon(flail); 

    Gnoll gnoll1("Gnoll Warrior", 30, 6, 0, 3);
    gnoll1.setHealingPotions(0);  
    gnoll1.equipWeapon(axe); 

    Gnoll gnoll2("Gnoll Scout", 30, 4, 0, 3);
    gnoll2.setHealingPotions(0);
    gnoll2.equipWeapon(knife);

    CombatManager combat;
    combat.addCombatant(&barbarian, true);
    combat.addCombatant(&rogue, true);
    combat.addCombatant(&wizard, true);
    combat.addCombatant(&gnollLeader, false);
    combat.addCombatant(&gnoll1, false);
    combat.addCombatant(&gnoll2, false);

    combat.startCombat();

    CombatVisualizer visualizer(&combat);
    if (visualizer.initialize()) {
        visualizer.run();
    }
    else {
        std::cout << "Failed to initialize visualizer\n";
        return 1;
    }

    return 0;
}