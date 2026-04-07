//
// Created by kamil on 21.03.2026.
//

#include "../../include/controller/HardAIController.h"

int HardAIController::getAttackWeight() const {
    return 60;
}

int HardAIController::getDefendWeight() const {
    return 60;
}

int HardAIController::getSpecialActionWeight() const {
    return 50;
}

EAction HardAIController::chooseAction(Character &self, Character &enemy) {
    if (enemy.getHp() <= self.getAttack()) {
        return EAction::ATTACK;  // zawsze dobija
    }
    return AIController::chooseAction(self, enemy);
}

