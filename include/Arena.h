//
// Created by kamil on 16.03.2026.
//

#ifndef ARENA_ARENA_H
#define ARENA_ARENA_H
#include "EAction.h"
#include "character/Character.h"
#include "controller/Controller.h"

class Arena {
private:
    Character* player1;
    Character* player2;
    Controller* controller1;
    Controller* controller2;
    int turn;

public:
    Arena(Character* p1, Character* p2, Controller* c1, Controller* c2);

    void startGame();
    void executeTurn(Character& attacker, Character& defender, Controller* controller);
    void executeAction(EAction action, Character& attacker, Character& defender);
    void printStatus() const;
};

#endif //ARENA_ARENA_H