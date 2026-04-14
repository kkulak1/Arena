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
    struct GameSetup {
        std::unique_ptr<Character> player1;
        std::unique_ptr<Character> player2;
        std::unique_ptr<Controller> controller1;
        std::unique_ptr<Controller> controller2;
        int turn = 1;
        int mode = 2;
        int aiDifficulty = 1;
    };

    int showMenu();
    int chooseMode();
    Controller* chooseAIDifficulty();
    Character* createCharacter(int playerNumber);
    void startMatch(Arena* arena );

    GameSetup setupNewGame();
    GameSetup setupLoadGameOrFallback();


public:
    void startGame();
};

#endif //ARENA_GAME_H