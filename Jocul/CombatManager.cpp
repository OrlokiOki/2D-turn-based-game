#include "CombatManager.h"
#include "GnollLeader.h"
#include "Gnoll.h"
#include <iostream>
#include <cstdlib>
#include <climits>

bool compareInitiative(const Combatant& a, const Combatant& b) {
    if (a.initiative != b.initiative) {
        return a.initiative > b.initiative;
    }
    if (a.isPlayer && !b.isPlayer) {
        return true;
    }
    return false;
}

CombatManager::CombatManager()
    : currentState(CombatState::INITIATIVE_ROLL), currentTurnIndex(0), fullRoundCounter(0) {
}

void CombatManager::addCombatant(Character* character, bool isPlayer) {
    combatants.push_back(Combatant(character, isPlayer));
}

void CombatManager::rollInitiative() {
    for (size_t i = 0; i < combatants.size(); i++) {
        combatants[i].initiative = (std::rand() % 4) + 1;
    }
}

void CombatManager::sortByInitiative() {
    std::sort(combatants.begin(), combatants.end(), compareInitiative);
}

void CombatManager::startCombat() {
    rollInitiative();
    sortByInitiative();
    currentTurnIndex = 0;
    fullRoundCounter = 0;

    std::cout << "\nCombat Started\n";
    std::cout << "Initiative Order:\n";
    for (size_t i = 0; i < combatants.size(); i++) {
        std::cout << i + 1 << ". ";
        if (combatants[i].isPlayer) {
            std::cout << "Player";
        }
        else {
            std::cout << "Enemy";
        }
        std::cout << " - " << combatants[i].character->getName()
            << " - Initiative: " << combatants[i].initiative << "\n";

        if (!combatants[i].isPlayer) {
            combatants[i].targetingStrategy = std::rand() % 2;
            std::cout << "   Targeting strategy: " << combatants[i].targetingStrategy << "\n";
        }
    }

    if (combatants[0].isPlayer) {
        currentState = CombatState::PLAYER_TURN;
    }
    else {
        currentState = CombatState::ENEMY_TURN;
    }
}

void CombatManager::processStatusEffects(Combatant& combatant) {
    combatant.character->updateStatusEffects();
}

Character* CombatManager::selectTargetLowestHp() {
    Character* target = NULL;
    int lowestHp = INT_MAX;

    for (size_t i = 0; i < combatants.size(); i++) {
        if (combatants[i].isPlayer && combatants[i].character->getHp() > 0) {
            if (combatants[i].character->getHp() < lowestHp) {
                lowestHp = combatants[i].character->getHp();
                target = combatants[i].character;
            }
        }
    }
    return target;
}

Character* CombatManager::selectTargetLowestDef() {
    Character* target = NULL;
    int lowestDef = INT_MAX;

    for (size_t i = 0; i < combatants.size(); i++) {
        if (combatants[i].isPlayer && combatants[i].character->getHp() > 0) {
            int totalDef = combatants[i].character->getBaseDef() +
                combatants[i].character->eqArmor.getDef();
            if (totalDef < lowestDef) {
                lowestDef = totalDef;
                target = combatants[i].character;
            }
        }
    }
    return target;
}

Character* CombatManager::selectTargetLowestInitiative() {
    Character* target = NULL;
    int lowestInitiative = INT_MAX;

    for (size_t i = 0; i < combatants.size(); i++) {
        if (combatants[i].isPlayer && combatants[i].character->getHp() > 0) {
            if (combatants[i].initiative < lowestInitiative) {
                lowestInitiative = combatants[i].initiative;
                target = combatants[i].character;
            }
        }
    }
    return target;
}

Character* CombatManager::selectTargetBelowThreshold() {
    for (size_t i = 0; i < combatants.size(); i++) {
        if (combatants[i].isPlayer && combatants[i].character->getHp() > 0) {
            float hpPercent = (float)combatants[i].character->getHp() /(float)combatants[i].character->getMaxHp();
            if (hpPercent < 0.3f) {
                std::cout << "Targeting " << combatants[i].character->getName()<< " (below 30% HP)\n";
                return combatants[i].character;
            }
        }
    }
    return NULL;
}

void CombatManager::enemyAI(Combatant& enemy) {
    Character* target = NULL;

    if (enemy.character->getName() == "Gnoll Leader") {
        GnollLeader* leader = static_cast<GnollLeader*>(enemy.character);

        if (!leader->hasStatusEffect(EffectType::INVISIBLE) && !leader->getHasUsedInvisPotion()) {
            leader->UseInvisPotion();
            std::cout << "Gnoll Leader used invisibility potion!\n";
            return;
        }

        float healthPercent = (float)leader->getHp() / (float)leader->getMaxHp();
        if (healthPercent < 0.4f && leader->getHealingPotions() > 0) {
            leader->UseHealingPotion();
            std::cout << "Gnoll Leader used a healing potion!\n";
            return;
        }

        if (fullRoundCounter > 0 && fullRoundCounter % 2 == 1 && !enemy.usedSpecialThisRound) {
            target = selectTargetBelowThreshold();
            if (target == NULL) {
                if (enemy.targetingStrategy == 1) {
                    target = selectTargetLowestInitiative();
                }
                else {
                    target = selectTargetLowestHp();
                }
            }
            if (target != NULL) {
                leader->BleedingAttack(*target);
                enemy.usedSpecialThisRound = true;
                std::cout << "Gnoll Leader used Bleeding Attack!\n";
                return;
            }
        }
    }
    target = selectTargetBelowThreshold();
    if (target == NULL) {
        if (enemy.targetingStrategy == 1) {
            target = selectTargetLowestInitiative();
            std::cout << "Enemy using strategy: Target lowest initiative\n";
        }
        else {
            if (enemy.character->getHp() == enemy.character->getMaxHp()) {
                target = selectTargetLowestDef();
            }
            else {
                target = selectTargetLowestHp();
            }
        }
    }
    if (target != NULL) {
        std::cout << "Enemy attacks " << target->getName() << "!\n";
        if (enemy.character->getName() == "Gnoll Leader") {
            GnollLeader* leader = static_cast<GnollLeader*>(enemy.character);
            leader->Attack(*target);
        }
        else {
            Gnoll* gnoll = static_cast<Gnoll*>(enemy.character);
            gnoll->Attack(*target);
        }
    }
}

void CombatManager::update() {
    if (currentState == CombatState::COMBAT_END) return;
    Combatant& current = combatants[currentTurnIndex];

    current.character->setTurnPoints(3);

    processStatusEffects(current);

    if (current.character->getHp() <= 0) {
        endTurn();
        return;
    }

    if (!current.isPlayer && currentState == CombatState::ENEMY_TURN) {
        enemyAI(current);
        endTurn();
    }
}

void CombatManager::endTurn() {

    bool playersAlive = false;
    bool enemiesAlive = false;

    for (size_t i = 0; i < combatants.size(); i++) {
        if (combatants[i].character->getHp() > 0) {
            if (combatants[i].isPlayer) playersAlive = true;
            else enemiesAlive = true;
        }
    }

    if (!playersAlive || !enemiesAlive) {
        currentState = CombatState::COMBAT_END;
        std::cout << "\nCombat Ended\n";
        if (playersAlive) std::cout << "Victory!\n";
        else std::cout << "Defeat!\n";
        return;
    }

    int startIndex = currentTurnIndex;
    do {
        currentTurnIndex = (currentTurnIndex + 1) % combatants.size();

        if (currentTurnIndex == 0) {
            fullRoundCounter++;
            std::cout << "\nFull Round " << fullRoundCounter << " Completed\n";

            for (size_t i = 0; i < combatants.size(); i++) {
                if (!combatants[i].isPlayer) {
                    combatants[i].usedSpecialThisRound = false;
                }
            }
        }

        // in caz de infinite loop
        if (currentTurnIndex == startIndex) break;

    } while (combatants[currentTurnIndex].character->getHp() <= 0);

    if (combatants[currentTurnIndex].isPlayer) {
        currentState = CombatState::PLAYER_TURN;
    }
    else {
        currentState = CombatState::ENEMY_TURN;
    }
}

bool CombatManager::isPlayerTurn() const {
    return currentState == CombatState::PLAYER_TURN;
}

bool CombatManager::isCombatOver() const {
    return currentState == CombatState::COMBAT_END;
}