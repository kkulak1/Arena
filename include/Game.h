//
// Created by kamil on 16.03.2026.
//

#ifndef ARENA_GAME_H
#define ARENA_GAME_H
#include "Arena.h"
#include "character/Character.h"

#include <memory>

class Game {
private:
    int showMenu();
    int chooseMode();
    Controller* chooseAIDifficulty();
    Character* createCharacter(int playerNumber);
    void startMatch(Arena* arena );
public:
    void startGame();
};

#endif //ARENA_GAME_H