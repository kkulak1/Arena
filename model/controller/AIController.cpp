//
// Created by kamil on 21.03.2026.
//

#include "../../include/controller/AIController.h"

namespace {
    constexpr int LOW_HP_DEFEND_BONUS = 20;
    constexpr int LOW_ENEMY_HP_ATTACK_BONUS = 50;
    constexpr int MID_GAME_SPECIAL_BONUS = 35;
}

EAction AIController::pickBest(int attack, int defend, int specialAction) {
    if (attack >= defend && attack >= specialAction) return EAction::ATTACK;
    if (defend >= specialAction) return EAction::DEFEND;
    return EAction::SPECIAL;
}

EAction AIController::chooseAction(Character &self, Character &enemy) {
    const double selfHp = static_cast<double>(self.getHp()) / static_cast<double>(self.getMaxHp());
    const double enemyHp = static_cast<double>(enemy.getHp()) / static_cast<double>(enemy.getMaxHp());
    const bool specialAvailable = self.canUseSpecial();

    int attackScore = getAttackWeight();
    int defendScore = getDefendWeight();
    int specialScore = getSpecialActionWeight();

    if (selfHp < 0.3) {
        defendScore += LOW_HP_DEFEND_BONUS;
    }
    if (enemyHp < 0.25) {
        attackScore += LOW_ENEMY_HP_ATTACK_BONUS;
    }
    if (specialAvailable && selfHp > 0.5 && enemyHp > 0.33) {
        specialScore += MID_GAME_SPECIAL_BONUS;
    }
    if (!specialAvailable) {
        specialScore = 0;
    }

    return pickBest(attackScore, defendScore, specialScore);
}

TurnDecision AIController::decideTurn(Character &self, Character &enemy) {
    TurnDecision decision;
    decision.action = chooseAction(self, enemy);
    return decision;
}