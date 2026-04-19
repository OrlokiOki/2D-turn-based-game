# Turn-Based Combat Game

---

## Overview
A D&D-inspired tactical combat game built with **C++ and SFML 3.0**. Control three heroes (Barbarian, Rogue, Wizard) against gnoll enemies in turn-based combat.

---

## Requirements
- **SFML 3.0+**
- **C++17 or later**
- Asset files: `ARCADECLASSIC.TTF`, `background.png`, character sprites (`orc_*.png`, `drow_*.png`, `tiefling_*.png`, `gnoll_*.png`)

---

## Compilation

```bash
g++ -std=c++17 -o combat_game *.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

---

## Run

```bash
./combat_game
```
---

## Gameplay
- **Turn Points (TP)**: Each character gets 3 TP per turn
- **Actions**: Attack (2-3 TP), Special (1 TP), Heal Potion (1 TP), Brace (2 TP)
- **Status Effects**: Bleeding (B), Invisible (I), Rage (R), Blindness (Bl)
- **Win Condition**: Defeat all enemies

---

## Pixelart
Drawn by me

## License
Educational project for learning purposes.
