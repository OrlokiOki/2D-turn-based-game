#include "CombatVisualizer.h"
#include "Barbarian.h"
#include "Rogue.h"
#include "Wizard.h"
#include "GnollLeader.h"
#include "Gnoll.h"
#include <iostream>

CombatVisualizer::CombatVisualizer(CombatManager* combat)
	: window(sf::VideoMode(sf::Vector2u(1328, 768)), "Turn-Based Combat"),
	combatManager(combat),
	backgroundSprite(NULL),
	turnText(NULL),
	selectedCharacterIndex(-1),
	selectedEnemyIndex(0),
	waitingForTargetSelection(false),
	selectedAttackIndex(-1),
	currentUIState(UIState::MAIN_ACTIONS),
	turnCounter(0),
	lastTurnCharacterIndex(-1) {
}

CombatVisualizer::~CombatVisualizer() {
	if (backgroundSprite) {
		delete backgroundSprite;
	}
	if (turnText) {
		delete turnText;
	}
	for (size_t i = 0; i < actionButtons.size(); i++) {
		delete actionButtons[i];
	}
	actionButtons.clear();
	for (size_t i = 0; i < attackSubButtons.size(); i++) {
		delete attackSubButtons[i];
	}
	attackSubButtons.clear();
	for (size_t i = 0; i < specialSubButtons.size(); i++) {
		delete specialSubButtons[i];
	}
	specialSubButtons.clear();
}

bool CombatVisualizer::initialize() {
	if (!font.openFromFile("ByteBounce.ttf")) {
		std::cout << "Warning: Could not load font ByteBounce.ttf\n";
	}

	gameView = window.getDefaultView();
	currentScale = 1.0f;
	viewOffset = sf::Vector2f(0.0f, 0.0f);
	window.setView(gameView);

	spriteManager.loadTexture("background", "background.png");
	spriteManager.loadTexture("orc_idle", "orc_idle.png");
	spriteManager.loadTexture("orc_attack", "orc_attack.png");
	spriteManager.loadTexture("orc_dead", "orc_dead.png");
	spriteManager.loadTexture("orc_damage_slashing", "orc_damage_slashing.png");
	spriteManager.loadTexture("orc_damage_cold", "orc_damage_cold.png");
	spriteManager.loadTexture("tiefling_idle", "tiefling_idle.png");
	spriteManager.loadTexture("tiefling_attack", "tiefling_attack.png");
	spriteManager.loadTexture("tiefling_dead", "tiefling_dead.png");
	spriteManager.loadTexture("tiefling_damage_slashing", "tiefling_damage_slashing.png");
	spriteManager.loadTexture("tiefling_damage_cold", "tiefling_damage_cold.png");
	spriteManager.loadTexture("drow_idle", "drow_idle.png");
	spriteManager.loadTexture("drow_attack", "drow_attack.png");
	spriteManager.loadTexture("drow_dead", "drow_dead.png");
	spriteManager.loadTexture("drow_damage_slashing", "drow_damage_slashing.png");
	spriteManager.loadTexture("drow_damage_cold", "drow_damage_cold.png");
	spriteManager.loadTexture("gnoll_idle", "gnoll_idle.png");
	spriteManager.loadTexture("gnoll_attack", "gnoll_attack.png");
	spriteManager.loadTexture("gnoll_dead", "gnoll_dead.png");
	spriteManager.loadTexture("gnoll_damage_slashing", "gnoll_damage_slashing.png");
	spriteManager.loadTexture("gnoll_damage_cold", "gnoll_damage_cold.png");

	sf::Texture* bgTexture = spriteManager.getTexture("background");
	if (bgTexture) {
		backgroundSprite = new sf::Sprite(*bgTexture);
		sf::Vector2u bgSize = bgTexture->getSize();
		float scaleX = 1328.0f / bgSize.x;
		float scaleY = 768.0f / bgSize.y;
		backgroundSprite->setScale(sf::Vector2f(scaleX, scaleY));
	}

	std::vector<Combatant>& combatants = combatManager->getAllCombatants();
	for (size_t i = 0; i < combatants.size(); i++) {
		CharacterSprite charSprite(combatants[i].character, combatants[i].isPlayer);
		sf::Texture* idleTexture = NULL;
		std::string spritePrefix = "";
		if (combatants[i].character->getName() == "Yotul") {
			spritePrefix = "orc";
			idleTexture = spriteManager.getTexture("orc_idle");
		}
		else if (combatants[i].character->getName() == "Vas") {
			spritePrefix = "drow";
			idleTexture = spriteManager.getTexture("drow_idle");
		}
		else if (combatants[i].character->getName() == "Clarity") {
			spritePrefix = "tiefling";
			idleTexture = spriteManager.getTexture("tiefling_idle");
		}
		else {
			spritePrefix = "gnoll";
			idleTexture = spriteManager.getTexture("gnoll_idle");
		}
		characterSpecialCooldowns[combatants[i].character] = 0;
		charSprite.setSpritePrefix(spritePrefix);
		charSprite.setTexture(idleTexture);
		characterSprites.push_back(charSprite);
	}
	positionCharacters();
	initializeUI();
	return true;
}

void CombatVisualizer::initializeUI() {
	float buttonWidth = 150.0f;
	float buttonHeight = 40.0f;
	float buttonSpacing = 10.0f;
	float startX = 50.0f;
	float startY = 680.0f;

	actionButtons.push_back(new UIButton("Attack", font, startX, startY, buttonWidth, buttonHeight));
	actionButtons.push_back(new UIButton("Special", font, startX + (buttonWidth + buttonSpacing), startY, buttonWidth, buttonHeight));
	actionButtons.push_back(new UIButton("Heal Potion", font, startX + 2 * (buttonWidth + buttonSpacing), startY, buttonWidth, buttonHeight));
	actionButtons.push_back(new UIButton("Brace", font, startX + 3 * (buttonWidth + buttonSpacing), startY, buttonWidth, buttonHeight));
	actionButtons.push_back(new UIButton("End Turn", font, startX + 4 * (buttonWidth + buttonSpacing), startY, buttonWidth, buttonHeight));

	attackSubButtons.push_back(new UIButton("Normal", font, startX, startY, buttonWidth, buttonHeight));
	attackSubButtons.push_back(new UIButton("Attack 2", font, startX + (buttonWidth + buttonSpacing), startY, buttonWidth, buttonHeight));
	attackSubButtons.push_back(new UIButton("Attack 3", font, startX + 2 * (buttonWidth + buttonSpacing), startY, buttonWidth, buttonHeight));
	attackSubButtons.push_back(new UIButton("Back", font, startX + 3 * (buttonWidth + buttonSpacing), startY, buttonWidth, buttonHeight));

	specialSubButtons.push_back(new UIButton("Special 1", font, startX, startY, buttonWidth, buttonHeight));
	specialSubButtons.push_back(new UIButton("Back", font, startX + (buttonWidth + buttonSpacing), startY, buttonWidth, buttonHeight));

	turnText = new sf::Text(font);
	turnText->setCharacterSize(24);
	turnText->setFillColor(sf::Color::Yellow);
	turnText->setPosition(sf::Vector2f(564.0f, 600.0f));
}

void CombatVisualizer::positionCharacters() {
	int playerCount = 0;
	int enemyCount = 0;
	for (size_t i = 0; i < characterSprites.size(); i++) {
		if (characterSprites[i].getIsPlayer()) {
			float x = 250.0f;
			float y = 150.0f + playerCount * 150.0f;
			characterSprites[i].setPosition(x, y);
			characterSprites[i].updateHPText(font);
			characterSprites[i].updateNameText(font);
			if (characterSprites[i].getCharacter()->getName() == "Clarity") {
				characterSprites[i].updateManaText(font);
			}
			characterSprites[i].updateStatusText(font);
			playerCount++;
		}
		else {
			float x = 1050.0f;
			float y = 150.0f + enemyCount * 150.0f;
			characterSprites[i].setPosition(x, y);
			characterSprites[i].updateHPText(font);
			characterSprites[i].updateNameText(font);
			characterSprites[i].updateStatusText(font);
			if (characterSprites[i].hasSprite()) {
				sf::Sprite& sprite = characterSprites[i].getSprite();
				sf::FloatRect bounds = sprite.getLocalBounds();
				sprite.setOrigin(sf::Vector2f(0, 0));
				sprite.setScale(sf::Vector2f(-3.0f, 3.0f));
				sprite.setPosition(sf::Vector2f(x, y));
			}
			enemyCount++;
		}
	}
}

std::string CombatVisualizer::getCharacterSpritePrefix(Character* character) {
	if (character->getName() == "Yotul") {
		return "orc";
	}
	else if (character->getName() == "Vas") {
		return "drow";
	}
	else if (character->getName() == "Clarity") {
		return "tiefling";
	}
	else {
		return "gnoll";
	}
}

void CombatVisualizer::hideAllButtons() {
	for (size_t i = 0; i < actionButtons.size(); i++) {
		actionButtons[i]->setEnabled(false);
	}
	for (size_t i = 0; i < attackSubButtons.size(); i++) {
		attackSubButtons[i]->setEnabled(false);
	}
	for (size_t i = 0; i < specialSubButtons.size(); i++) {
		specialSubButtons[i]->setEnabled(false);
	}
}

void CombatVisualizer::showMainActions() {
	currentUIState = UIState::MAIN_ACTIONS;
	if (!combatManager->isPlayerTurn()) {
		hideAllButtons();
		return;
	}
	Combatant& current = combatManager->getCurrentCombatant();
	int tp = current.character->getTurnPoints();
	int cooldown = characterSpecialCooldowns[current.character];

	actionButtons[0]->setEnabled(tp >= 2);
	actionButtons[1]->setEnabled(tp >= 1 && cooldown <= 0);
	actionButtons[2]->setEnabled(tp >= 1 && current.character->getHealingPotions() > 0);
	actionButtons[3]->setEnabled(tp >= 2);
	actionButtons[4]->setEnabled(true);

	for (size_t i = 0; i < attackSubButtons.size(); i++) {
		attackSubButtons[i]->setEnabled(false);
	}
	for (size_t i = 0; i < specialSubButtons.size(); i++) {
		specialSubButtons[i]->setEnabled(false);
	}
}

void CombatVisualizer::showSpecialSubmenu() {
	currentUIState = UIState::SPECIAL_SUBMENU;

	for (size_t i = 0; i < actionButtons.size(); i++) {
		actionButtons[i]->setEnabled(false);
	}

	for (size_t i = 0; i < attackSubButtons.size(); i++) {
		attackSubButtons[i]->setEnabled(false);
	}

	Combatant& current = combatManager->getCurrentCombatant();
	int tp = current.character->getTurnPoints();
	int cooldown = characterSpecialCooldowns[current.character];

	if (current.character->getName() == "Yotul") {
		specialSubButtons[0]->setEnabled(tp >= 1 && cooldown <= 0);
		specialSubButtons[0]->setLabel("Rage");
	}
	else if (current.character->getName() == "Clarity") {
		specialSubButtons[0]->setEnabled(tp >= 1 && cooldown <= 0);
		specialSubButtons[0]->setLabel("Mana Regen");
	}
	else if (current.character->getName() == "Vas") {
		specialSubButtons[0]->setEnabled(tp >= 1 && cooldown <= 0);
		specialSubButtons[0]->setLabel("Hide");
	}

	specialSubButtons[1]->setEnabled(true);
	specialSubButtons[1]->setLabel("Back");
}


void CombatVisualizer::showAttackSubmenu() {
	currentUIState = UIState::ATTACK_SUBMENU;
	for (size_t i = 0; i < actionButtons.size(); i++) {
		actionButtons[i]->setEnabled(false);
	}
	for (size_t i = 0; i < specialSubButtons.size(); i++) {
		specialSubButtons[i]->setEnabled(false);
	}

	Combatant& current = combatManager->getCurrentCombatant();
	int tp = current.character->getTurnPoints();

	if (current.character->getName() == "Yotul") {
		attackSubButtons[0]->setEnabled(tp >= 2);
		attackSubButtons[0]->setLabel("Normal");
		attackSubButtons[1]->setEnabled(tp >= 3);
		attackSubButtons[1]->setLabel("Swing");
		attackSubButtons[2]->setEnabled(false);
		attackSubButtons[2]->setLabel("");
		attackSubButtons[3]->setEnabled(true);
		attackSubButtons[3]->setLabel("Back");
	}
	else if (current.character->getName() == "Clarity") {
		Wizard* wizard = static_cast<Wizard*>(current.character);
		attackSubButtons[0]->setEnabled(tp >= 2);
		attackSubButtons[0]->setLabel("Ice Spike");
		attackSubButtons[1]->setEnabled(tp >= 2 && wizard->getManaPool() >= 10);
		attackSubButtons[1]->setLabel("Magic Missile");
		attackSubButtons[2]->setEnabled(tp >= 3 && wizard->getManaPool() >= 20);
		attackSubButtons[2]->setLabel("Blizzard");
		attackSubButtons[3]->setEnabled(true);
		attackSubButtons[3]->setLabel("Back");
	}
	else if (current.character->getName() == "Vas") {
		attackSubButtons[0]->setEnabled(tp >= 2);
		attackSubButtons[0]->setLabel("Normal");
		attackSubButtons[1]->setEnabled(tp >= 3);
		attackSubButtons[1]->setLabel("Smoke Powder");
		attackSubButtons[2]->setEnabled(false);
		attackSubButtons[2]->setLabel("");
		attackSubButtons[3]->setEnabled(true);
		attackSubButtons[3]->setLabel("Back");
	}
}

void CombatVisualizer::updateCharacterStates() {
	for (size_t i = 0; i < characterSprites.size(); i++) {
		Character* ch = characterSprites[i].getCharacter();
		characterSprites[i].updateHPText(font);
		if (characterSprites[i].getCharacter()->getName() == "Clarity") {
			characterSprites[i].updateManaText(font);
		}
		characterSprites[i].updateStatusText(font);
		AnimationState currentState = characterSprites[i].getState();
		std::string spritePrefix = characterSprites[i].getSpritePrefix();

		if (ch->getHp() <= 0) {
			if (currentState != AnimationState::DEAD) {
				characterSprites[i].setState(AnimationState::DEAD);
				sf::Texture* deadTexture = spriteManager.getTexture(spritePrefix + "_dead");
				characterSprites[i].setTexture(deadTexture);
			}
		}
		else {
			if (currentState == AnimationState::IDLE) {
				sf::Texture* idleTexture = spriteManager.getTexture(spritePrefix + "_idle");
				characterSprites[i].setTexture(idleTexture);
			}
			else if (currentState == AnimationState::ATTACK) {
				sf::Texture* attackTexture = spriteManager.getTexture(spritePrefix + "_attack");
				characterSprites[i].setTexture(attackTexture);
			}
			else if (currentState == AnimationState::DAMAGE_SLASHING) {
				sf::Texture* damageTexture = spriteManager.getTexture(spritePrefix + "_damage_slashing");
				characterSprites[i].setTexture(damageTexture);
			}
			else if (currentState == AnimationState::DAMAGE_COLD) {
				sf::Texture* damageTexture = spriteManager.getTexture(spritePrefix + "_damage_cold");
				characterSprites[i].setTexture(damageTexture);
			}
		}
	}
}

void CombatVisualizer::updateUI() {
	if (combatManager->isPlayerTurn()) {
		Combatant& current = combatManager->getCurrentCombatant();
		std::string turnString = current.character->getName() + "'s Turn (TP: " +
			std::to_string(current.character->getTurnPoints()) + ")";
		turnText->setString(turnString);

		if (currentUIState == UIState::MAIN_ACTIONS) {
			showMainActions();
		}
		else if (currentUIState == UIState::ATTACK_SUBMENU) {
			showAttackSubmenu();
		}
		else if (currentUIState == UIState::SPECIAL_SUBMENU) {
			showSpecialSubmenu();
		}
	}
	else {
		turnText->setString("Enemy Turn");
		hideAllButtons();
	}
}


void CombatVisualizer::handleMouseClick(float x, float y) {
	if (!combatManager->isPlayerTurn()) return;
	Combatant& current = combatManager->getCurrentCombatant();

	for (size_t j = 0; j < characterSprites.size(); j++) {
		if (characterSprites[j].getCharacter() == current.character) {
			selectedCharacterIndex = j;
			break;
		}
	}

	if (waitingForTargetSelection) {
		for (size_t i = 0; i < characterSprites.size(); i++) {
			if (!characterSprites[i].getIsPlayer() &&
				characterSprites[i].getCharacter()->getHp() > 0 &&
				characterSprites[i].containsPoint(x, y)) {
				selectedEnemyIndex = i;
				Character* attacker = current.character;
				Character* target = characterSprites[selectedEnemyIndex].getCharacter();

				if (selectedCharacterIndex >= 0) {
					characterSprites[selectedCharacterIndex].playAttackAnimation();
					std::string attackerPrefix = characterSprites[selectedCharacterIndex].getSpritePrefix();
					sf::Texture* attackTexture = spriteManager.getTexture(attackerPrefix + "_attack");
					characterSprites[selectedCharacterIndex].setTexture(attackTexture);
				}

				render();
				sf::sleep(sf::seconds(1.0f));

				if (attacker->getName() == "Clarity" && selectedAttackIndex == 1) {
					Wizard* wizard = static_cast<Wizard*>(attacker);
					wizard->MagicMissile(*target);
				}
				else {
					if (attacker->getName() == "Yotul") {
						static_cast<Barbarian*>(attacker)->Attack(*target);
					}
					else if (attacker->getName() == "Vas") {
						static_cast<Rogue*>(attacker)->Attack(*target);
					}
					else if (attacker->getName() == "Clarity") {
						static_cast<Wizard*>(attacker)->Attack(*target);
					}
				}

				DamageType damageType = target->getLastDamageType();
				characterSprites[selectedEnemyIndex].playDamageAnimation(damageType);

				std::string targetPrefix = characterSprites[selectedEnemyIndex].getSpritePrefix();
				sf::Texture* damageTexture = NULL;
				if (damageType == DamageType::SLASHING || damageType == DamageType::PIERCING || damageType == DamageType::BLUDGEONING) {
					damageTexture = spriteManager.getTexture(targetPrefix + "_damage_slashing");
				}
				else if (damageType == DamageType::COLD || damageType == DamageType::FIRE || damageType == DamageType::LIGHTNING || damageType == DamageType::RADIANT || damageType == DamageType::FORCE) {
					damageTexture = spriteManager.getTexture(targetPrefix + "_damage_cold");
				}
				if (damageTexture) {
					characterSprites[selectedEnemyIndex].setTexture(damageTexture);
				}

				render();
				sf::sleep(sf::seconds(1.0f));

				waitingForTargetSelection = false;
				selectedAttackIndex = -1;
				showMainActions();
				return;
			}
		}
	}

	if (currentUIState == UIState::MAIN_ACTIONS) {
		for (size_t i = 0; i < actionButtons.size(); i++) {
			if (actionButtons[i]->contains(x, y) && actionButtons[i]->isButtonEnabled()) {
				if (i == 0) {
					showAttackSubmenu();
				}
				else if (i == 1) {
					showSpecialSubmenu();
				}
				else if (i == 2) {
					current.character->UseHealingPotion();
				}
				else if (i == 3) {
					current.character->Brace();
				}
				else if (i == 4) {
					combatManager->endTurn();
					turnCounter++;
					if (turnCounter % 6 == 0) {
						for (std::map<Character*, int>::iterator it = characterSpecialCooldowns.begin();
							it != characterSpecialCooldowns.end(); ++it) {
							if (it->second > 0) {
								it->second--;
							}
						}
					}
					showMainActions();
				}
				return;
			}
		}
	}
	if (currentUIState == UIState::SPECIAL_SUBMENU) {
		for (size_t i = 0; i < specialSubButtons.size(); i++) {
			if (specialSubButtons[i]->contains(x, y) && specialSubButtons[i]->isButtonEnabled()) {
				if (i == 1) {
					showMainActions();
				}
				else if (i == 0) {
					if (current.character->getName() == "Yotul") {
						Barbarian* barbarian = static_cast<Barbarian*>(current.character);
						barbarian->StartRage();
						characterSpecialCooldowns[current.character] = 3;
						std::cout << "Barbarian activated Rage!\n";
					}
					else if (current.character->getName() == "Clarity") {
						Wizard* wizard = static_cast<Wizard*>(current.character);
						int currentMana = wizard->getManaPool();
						wizard->setManaPool(currentMana + 20);
						wizard->setTurnPoints(wizard->getTurnPoints() - 1);
						characterSpecialCooldowns[current.character] = 3;
						std::cout << "Wizard regenerated 20 mana!\n";
					}
					else if (current.character->getName() == "Vas") {
						Rogue* rogue = static_cast<Rogue*>(current.character);
						rogue->Hide();
						characterSpecialCooldowns[current.character] = 3;
						std::cout << "Rogue used Hide!\n";
					}
					showMainActions();
				}
				return;
			}
		}
	}

	if (currentUIState == UIState::ATTACK_SUBMENU) {
		for (size_t i = 0; i < attackSubButtons.size(); i++) {
			if (attackSubButtons[i]->contains(x, y) && attackSubButtons[i]->isButtonEnabled()) {
				if (i == 3) {
					showMainActions();
				}
				else if (i == 0) {
					selectedAttackIndex = 0;
					waitingForTargetSelection = true;
					std::cout << "Click on an enemy to attack!\n";
				}
				else if (i == 1) {
					if (current.character->getName() == "Yotul") {
						std::vector<Character*> enemies;
						for (size_t j = 0; j < characterSprites.size(); j++) {
							if (!characterSprites[j].getIsPlayer() &&
								characterSprites[j].getCharacter()->getHp() > 0) {
								enemies.push_back(characterSprites[j].getCharacter());
							}
						}
						if (selectedCharacterIndex >= 0) {
							characterSprites[selectedCharacterIndex].playAttackAnimation();
							std::string attackerPrefix = characterSprites[selectedCharacterIndex].getSpritePrefix();
							sf::Texture* attackTexture = spriteManager.getTexture(attackerPrefix + "_attack");
							characterSprites[selectedCharacterIndex].setTexture(attackTexture);
						}
						render();
						sf::sleep(sf::seconds(1.0f));

						static_cast<Barbarian*>(current.character)->Swing(enemies);

						for (size_t j = 0; j < characterSprites.size(); j++) {
							if (!characterSprites[j].getIsPlayer() &&
								characterSprites[j].getCharacter()->getHp() > 0) {
								DamageType dmgType = characterSprites[j].getCharacter()->getLastDamageType();
								characterSprites[j].playDamageAnimation(dmgType);
								std::string prefix = characterSprites[j].getSpritePrefix();
								sf::Texture* dmgTex = spriteManager.getTexture(prefix + "_damage_slashing");
								if (dmgTex) characterSprites[j].setTexture(dmgTex);
							}
						}
						render();
						sf::sleep(sf::seconds(1.0f));

						showMainActions();
					}
					else if (current.character->getName() == "Clarity") {
						selectedAttackIndex = 1;
						waitingForTargetSelection = true;
						std::cout << "Click on an enemy for Magic Missile!\n";
					}
					else if (current.character->getName() == "Vas") {
						std::vector<Character*> enemies;
						for (size_t j = 0; j < characterSprites.size(); j++) {
							if (!characterSprites[j].getIsPlayer() &&
								characterSprites[j].getCharacter()->getHp() > 0) {
								enemies.push_back(characterSprites[j].getCharacter());
							}
						}
						if (selectedCharacterIndex >= 0) {
							characterSprites[selectedCharacterIndex].playAttackAnimation();
							std::string attackerPrefix = characterSprites[selectedCharacterIndex].getSpritePrefix();
							sf::Texture* attackTexture = spriteManager.getTexture(attackerPrefix + "_attack");
							characterSprites[selectedCharacterIndex].setTexture(attackTexture);
						}
						render();
						sf::sleep(sf::seconds(1.0f));

						static_cast<Rogue*>(current.character)->SmokePowder(enemies);

						for (size_t j = 0; j < characterSprites.size(); j++) {
							if (!characterSprites[j].getIsPlayer()) {
								characterSprites[j].playDamageAnimation(DamageType::POISON);
								std::string prefix = characterSprites[j].getSpritePrefix();
								sf::Texture* dmgTex = spriteManager.getTexture(prefix + "_damage_cold");
								if (dmgTex) characterSprites[j].setTexture(dmgTex);
							}
						}
						render();
						sf::sleep(sf::seconds(1.0f));

						showMainActions();
					}
				}
				else if (i == 2) {
					if (current.character->getName() == "Clarity") {
						std::vector<Character*> enemies;
						for (size_t j = 0; j < characterSprites.size(); j++) {
							if (!characterSprites[j].getIsPlayer() &&
								characterSprites[j].getCharacter()->getHp() > 0) {
								enemies.push_back(characterSprites[j].getCharacter());
							}
						}
						if (selectedCharacterIndex >= 0) {
							characterSprites[selectedCharacterIndex].playAttackAnimation();
							std::string attackerPrefix = characterSprites[selectedCharacterIndex].getSpritePrefix();
							sf::Texture* attackTexture = spriteManager.getTexture(attackerPrefix + "_attack");
							characterSprites[selectedCharacterIndex].setTexture(attackTexture);
						}
						render();
						sf::sleep(sf::seconds(1.0f));

						static_cast<Wizard*>(current.character)->Blizzard(enemies);

						for (size_t j = 0; j < characterSprites.size(); j++) {
							if (!characterSprites[j].getIsPlayer() &&
								characterSprites[j].getCharacter()->getHp() > 0) {
								characterSprites[j].playDamageAnimation(DamageType::COLD);
								std::string prefix = characterSprites[j].getSpritePrefix();
								sf::Texture* dmgTex = spriteManager.getTexture(prefix + "_damage_cold");
								if (dmgTex) characterSprites[j].setTexture(dmgTex);
							}
						}
						render();
						sf::sleep(sf::seconds(1.0f));

						showMainActions();
					}
				}
				return;
			}
		}
	}
}

void CombatVisualizer::render() {
	window.clear();
	if (backgroundSprite) {
		window.draw(*backgroundSprite);
	}
	for (size_t i = 0; i < characterSprites.size(); i++) {
		if (characterSprites[i].hasSprite()) {
			window.draw(characterSprites[i].getSprite());
			if (characterSprites[i].getHPText()) {
				window.draw(*characterSprites[i].getHPText());
			}
			if (characterSprites[i].getNameText()) {
				window.draw(*characterSprites[i].getNameText());
			}
			if (characterSprites[i].getManaText()) {
				window.draw(*characterSprites[i].getManaText());
			}
			std::vector<sf::Text*>& statusTexts = characterSprites[i].getStatusTexts();
			for (size_t j = 0; j < statusTexts.size(); j++) {
				if (statusTexts[j]) {
					window.draw(*statusTexts[j]);
				}
			}
		}
	}
	if (turnText) {
		window.draw(*turnText);
	}

	if (currentUIState == UIState::MAIN_ACTIONS) {
		for (size_t i = 0; i < actionButtons.size(); i++) {
			actionButtons[i]->draw(window);
		}
	}
	else if (currentUIState == UIState::ATTACK_SUBMENU) {
		for (size_t i = 0; i < attackSubButtons.size(); i++) {
			attackSubButtons[i]->draw(window);
		}
	}
	else if (currentUIState == UIState::SPECIAL_SUBMENU) {
		for (size_t i = 0; i < specialSubButtons.size(); i++) {
			specialSubButtons[i]->draw(window);
		}
	}

	window.display();
}

void CombatVisualizer::run() {
	gameView = window.getDefaultView();
	currentScale = 1.0f;
	viewOffset = sf::Vector2f(0.0f, 0.0f);

	while (window.isOpen() && !combatManager->isCombatOver()) {
		float deltaTime = clock.restart().asSeconds();
		while (const std::optional<sf::Event> event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::F11) {
					static bool isFullscreen = false;
					if (!isFullscreen) {

						window.create(sf::VideoMode::getFullscreenModes()[0], "Turn-Based Combat",
							sf::State::Fullscreen);
						isFullscreen = true;
					}
					else {
						window.create(sf::VideoMode(sf::Vector2u(1328, 768)), "Turn-Based Combat",
							sf::State::Windowed);
						isFullscreen = false;
					}

					sf::Vector2u windowSize = window.getSize();
					float baseWidth = 1328.0f;
					float baseHeight = 768.0f;

					float scaleX = static_cast<float>(windowSize.x) / baseWidth;
					float scaleY = static_cast<float>(windowSize.y) / baseHeight;
					if (scaleX < scaleY) {
						currentScale = scaleX;
					}
					else {
						currentScale = scaleY;
					}

					float viewWidth = baseWidth * currentScale;
					float viewHeight = baseHeight * currentScale;

					// letterboxing stuff
					viewOffset.x = (windowSize.x - viewWidth) / 2.0f;
					viewOffset.y = (windowSize.y - viewHeight) / 2.0f;

					gameView.setSize(sf::Vector2f(baseWidth, baseHeight));
					gameView.setCenter(sf::Vector2f(baseWidth / 2.0f, baseHeight / 2.0f));

					sf::FloatRect viewport(
						sf::Vector2f(viewOffset.x / windowSize.x, viewOffset.y / windowSize.y),
						sf::Vector2f(viewWidth / windowSize.x, viewHeight / windowSize.y)
					);
					gameView.setViewport(viewport);
					window.setView(gameView);

					if (backgroundSprite) {
						sf::Texture* bgTexture = spriteManager.getTexture("background");
						if (bgTexture) {
							sf::Vector2u bgSize = bgTexture->getSize();
							float bgScaleX = baseWidth / bgSize.x;
							float bgScaleY = baseHeight / bgSize.y;
							backgroundSprite->setScale(sf::Vector2f(bgScaleX, bgScaleY));
							backgroundSprite->setPosition(sf::Vector2f(0.0f, 0.0f));
						}
					}
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, gameView);
				handleMouseClick(worldPos.x, worldPos.y);
				sf::sleep(sf::seconds(0.2f));
			}
		}

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, gameView);

		if (currentUIState == UIState::MAIN_ACTIONS) {
			for (size_t i = 0; i < actionButtons.size(); i++) {
				actionButtons[i]->updateHover(worldPos.x, worldPos.y);
			}
		}
		else if (currentUIState == UIState::ATTACK_SUBMENU) {
			for (size_t i = 0; i < attackSubButtons.size(); i++) {
				attackSubButtons[i]->updateHover(worldPos.x, worldPos.y);
			}
		}
		else if (currentUIState == UIState::SPECIAL_SUBMENU) {
			for (size_t i = 0; i < specialSubButtons.size(); i++) {
				specialSubButtons[i]->updateHover(worldPos.x, worldPos.y);
			}
		}

		for (size_t i = 0; i < characterSprites.size(); i++) {
			characterSprites[i].updateAnimation(deltaTime);
		}

		if (!combatManager->isPlayerTurn() && !combatManager->isCombatOver()) {
			Combatant& current = combatManager->getCurrentCombatant();
			int enemyIndex = -1;
			for (size_t i = 0; i < characterSprites.size(); i++) {
				if (characterSprites[i].getCharacter() == current.character) {
					enemyIndex = i;
					break;
				}
			}
			if (enemyIndex >= 0 && enemyIndex < (int)characterSprites.size()) {
				characterSprites[enemyIndex].playAttackAnimation();
				std::string spritePrefix = getCharacterSpritePrefix(current.character);
				sf::Texture* attackTexture = spriteManager.getTexture(spritePrefix + "_attack");
				characterSprites[enemyIndex].setTexture(attackTexture);
			}
			render();
			sf::sleep(sf::seconds(1.0f));

			std::vector<int> playerHpBefore;
			for (size_t i = 0; i < characterSprites.size(); i++) {
				if (characterSprites[i].getIsPlayer()) {
					playerHpBefore.push_back(characterSprites[i].getCharacter()->getHp());
				}
				else {
					playerHpBefore.push_back(-1);
				}
			}

			combatManager->update();

			for (size_t i = 0; i < characterSprites.size(); i++) {
				if (characterSprites[i].getIsPlayer() && playerHpBefore[i] != -1) {
					int hpAfter = characterSprites[i].getCharacter()->getHp();
					if (hpAfter < playerHpBefore[i]) {
						DamageType dmgType = characterSprites[i].getCharacter()->getLastDamageType();
						characterSprites[i].playDamageAnimation(dmgType);
						std::string prefix = characterSprites[i].getSpritePrefix();
						sf::Texture* damageTexture = NULL;
						if (dmgType == DamageType::SLASHING ||dmgType == DamageType::PIERCING ||dmgType == DamageType::BLUDGEONING) {
							damageTexture = spriteManager.getTexture(prefix + "_damage_slashing");
						}
						else {
							damageTexture = spriteManager.getTexture(prefix + "_damage_cold");
						}
						if (damageTexture) {
							characterSprites[i].setTexture(damageTexture);
						}
						break;
					}
				}
			}

			render();
			sf::sleep(sf::seconds(0.5f));
		}

		updateCharacterStates();
		updateUI();

		if (combatManager->isPlayerTurn()) {
			Combatant& current = combatManager->getCurrentCombatant();
			int currentCharIndex = -1;
			for (size_t i = 0; i < characterSprites.size(); i++) {
				if (characterSprites[i].getCharacter() == current.character) {
					currentCharIndex = i;
					break;
				}
			}
			if (currentCharIndex != lastTurnCharacterIndex) {
				current.character->setTurnPoints(3);
				lastTurnCharacterIndex = currentCharIndex;
			}
		}

		render();
	}

	if (combatManager->isCombatOver()) {
		std::cout << "Combat Over!\n";
		sf::sleep(sf::seconds(2.0f));
	}
}