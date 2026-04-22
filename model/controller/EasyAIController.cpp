//
// Created by kamil on 16.03.2026.
//

#include "../../include/controller/EasyAIController.h"
#include <vector>
#include "templates/WeightedPicker.h"

enum class EasyMode {
    RANDOM,
    NORMAL
};

int EasyAIController::getAttackWeight() const {
    return 30;
}

int EasyAIController::getDefendWeight() const {
    return 35;
}

int EasyAIController::getSpecialActionWeight() const {
    return 10;
}

EAction EasyAIController::chooseAction(Character &self, Character &enemy) {
    std::vector<std::pair<EasyMode, int>> mode = {
        {EasyMode::RANDOM, 60},
        {EasyMode::NORMAL, 40}
    };

    if (pickWeightedRandom(mode, EasyMode::NORMAL) == EasyMode::RANDOM) {
        std::vector<std::pair<EAction, int>> randomActions = {
            {EAction::ATTACK, 3},
            {EAction::DEFEND, 4},
            {EAction::SPECIAL, 1}
        };
        return pickWeightedRandom(randomActions, EAction::ATTACK);
    }
    return AIController::chooseAction(self, enemy);
}
