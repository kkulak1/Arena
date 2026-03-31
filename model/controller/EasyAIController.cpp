//
// Created by kamil on 16.03.2026.
//

#include "../../include/controller/EasyAIController.h"

int EasyAIController::getAttackWeight() const {
    return 40;
}

int EasyAIController::getDefendWeight() const {
    return 30;
}

int EasyAIController::getSpecialActionWeight() const {
    return 20;
}

Action EasyAIController::chooseAction(Character &self, Character &enemy) {
    if (rand() % 100 < 30) {
        return static_cast<Action>(rand() % 3); // wybierz losowo
    }
    return AIController::chooseAction(self, enemy);
}
