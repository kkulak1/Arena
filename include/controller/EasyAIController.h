//
// Created by kamil on 16.03.2026.
//

#ifndef ARENA_EASYAICONTROLLER_H
#define ARENA_EASYAICONTROLLER_H
#include "AIController.h"


class EasyAIController :public AIController {
protected:
    int getAttackWeight() const override;
    int getDefendWeight() const override;
    int getSpecialActionWeight() const override;
public:
    EAction chooseAction(Character &self, Character &enemy) override;
};


#endif //ARENA_EASYAICONTROLLER_H