//
// Created by kamil on 21.03.2026.
//

#ifndef ARENA_HARDAICONTROLLER_H
#define ARENA_HARDAICONTROLLER_H
#include "AIController.h"
#include "Controller.h"


class HardAIController : public AIController{
protected:
    int getAttackWeight() const override;
    int getDefendWeight() const override;
    int getSpecialActionWeight() const override;
public:
    EAction chooseAction(Character &self, Character &enemy) override;
};


#endif //ARENA_HARDAICONTROLLER_H