//
// Created by kamil on 21.03.2026.
//

#ifndef ARENA_AICONTROLLER_H
#define ARENA_AICONTROLLER_H
#include "Controller.h"


class AIController : public Controller {
protected:
    virtual int getAttackWeight() const = 0;
    virtual int getDefendWeight() const = 0;
    virtual int getSpecialActionWeight() const = 0;

    EAction pickBest(int attack, int defend, int specialAction);
public:
    TurnDecision decideTurn(Character &self, Character &enemy) override;
    virtual EAction chooseAction(Character &self, Character &enemy);
};


#endif //ARENA_AICONTROLLER_H