#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include "Character.h"

struct Combatant {
    Character* character;
    int initiative;
    bool isPlayer;
    int targetingStrategy;  
    bool usedSpecialThisRound;  

    Combatant(Character* c, bool player)
        : character(c), initiative(0), isPlayer(player),
        targetingStrategy(0), usedSpecialThisRound(false) {
    }
};

enum class CombatState {
    INITIATIVE_ROLL,
    PLAYER_TURN,
    ENEMY_TURN,
    TURN_END,
    COMBAT_END
};

bool compareInitiative(const Combatant& a, const Combatant& b);

class CombatManager {
private:
    std::vector<Combatant> combatants;
    CombatState currentState;
    int currentTurnIndex;
    int fullRoundCounter;

    void rollInitiative();
    void sortByInitiative();
    void processStatusEffects(Combatant& combatant);
    void enemyAI(Combatant& enemy);

    Character* selectTargetLowestHp();
    Character* selectTargetLowestDef();
    Character* selectTargetLowestInitiative();
    Character* selectTargetBelowThreshold();

public:
    CombatManager();

    void addCombatant(Character* character, bool isPlayer);
    void startCombat();
    void update();
    void endTurn();

    CombatState getState() const { return currentState; }
    Combatant& getCurrentCombatant() { return combatants[currentTurnIndex]; }
    std::vector<Combatant>& getAllCombatants() { return combatants; }

    bool isPlayerTurn() const;
    bool isCombatOver() const;
};