#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <map>
#include "CharacterSprite.h"
#include "UIButton.h"
#include "CombatManager.h"
#include "SpriteManager.h"

enum class UIState {
    MAIN_ACTIONS,
    ATTACK_SUBMENU,
    SPECIAL_SUBMENU
};


class CombatVisualizer {
private:
    sf::RenderWindow window;
    SpriteManager spriteManager;
    sf::Font font;
    sf::Sprite* backgroundSprite;
    std::vector<CharacterSprite> characterSprites;
    std::vector<UIButton*> actionButtons;
    std::vector<UIButton*> attackSubButtons;
    CombatManager* combatManager;
    sf::Clock clock;
    int selectedAttackIndex;
    std::vector<UIButton*> specialSubButtons;

    sf::Text* turnText;
    int selectedCharacterIndex;
    int selectedEnemyIndex;
    bool waitingForTargetSelection;
    UIState currentUIState;

    sf::View gameView;
    float currentScale;
    sf::Vector2f viewOffset;

    std::map<Character*, int> characterSpecialCooldowns;
    int turnCounter;
    int lastTurnCharacterIndex;
   

    void initializeUI();
    void updateUI();
    void handleMouseClick(float x, float y);
    void positionCharacters();
    void updateCharacterStates();
    std::string getCharacterSpritePrefix(Character* character);

    void showMainActions();
    void showAttackSubmenu();
    void hideAllButtons();
    void showSpecialSubmenu();

public:
    CombatVisualizer(CombatManager* combat);
    ~CombatVisualizer();

    bool initialize();
    void run();
    void render();
};