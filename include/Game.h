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
    static int showMenu();
    int chooseMode();
    std::unique_ptr<Controller> chooseAIDifficulty();
    std::unique_ptr<Character> createCharacter(int playerNumber);

    bool handleLoadGame(std::unique_ptr<Character>& player1, std::unique_ptr<Character>& player2, std::unique_ptr<Controller>& controller2, int& turn, int& mode, int& aiDifficulty);
    void handleNewGame(std::unique_ptr<Character>& player1, std::unique_ptr<Character>& player2, std::unique_ptr<Controller>& controller2, int& mode, int& aiDifficulty);
    void runGameLoop(Character* player1, Character* player2, Controller* controller1, Controller* controller2, int& turn, int mode, int aiDifficulty);
public:
    void startGame();
};

#endif //ARENA_GAME_H