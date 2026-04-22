//
// Created by kamil on 16.03.2026.
//

#ifndef ARENA_ARENA_H
#define ARENA_ARENA_H
#include "types/TurnDecision.h"
#include "character/Character.h"
#include "controller/Controller.h"

class Arena {
private:
    Character* player1;
    Character* player2;
    Controller* controller1;
    Controller* controller2;
    int turn;
    int mode = 2;
    int aiDifficulty = 1;
    bool shouldExit = false;

    static int applyDamage(Character& defender, int damage);
    void saveCurrentGame() const;

public:
    Arena(Character* p1, Character* p2, Controller* c1, Controller* c2, int startTurn = 1, int mode = 2, int aiDifficulty = 1);

    void startGame();
    void executeTurn(Character& attacker, Character& defender, Controller* controller);
    void executeAction(EAction action, Character& attacker, Character& defender);

    static void applyAttack(Character& attacker, Character& defender);
    static void applyDefending(Character& defender);
    void applySpecialAbility(Character& attacker, Character& defender);
    void printStatus() const;
};

#endif //ARENA_ARENA_H