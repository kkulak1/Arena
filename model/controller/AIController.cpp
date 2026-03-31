//
// Created by kamil on 21.03.2026.
//

#include "../../include/controller/AIController.h"

Action AIController::pickBest(int attack, int defend, int specialAction) {
    if (attack >= defend && attack >= specialAction) return Action::ATTACK;
    if (defend >= specialAction) return Action::DEFEND;
    return Action::SPECIAL;
}

Action AIController::chooseAction(Character &self, Character &enemy) {
    float selfHp = (float) self.getHp() / self.getMaxHp();
    float enemyHp = (float) enemy.getHp() / enemy.getMaxHp();

    int attackScore = getAttackWeight();
    int defendScore = 0;
    int specialScore = 0;

    if (selfHp < 0.3f) {
        defendScore += getDefendWeight();
    }
    if (enemyHp < 0.25f) {
        attackScore += 50;
    }

    return pickBest(attackScore, defendScore, specialScore);
}
