//
// Created by kamil on 16.03.2026.
//

#include "../../include/controller/AIController.h"

#include "../../include/Action.h"
#include "../../include/character/Character.h"

Action AIController::chooseAction(Character &character) {
    if (character.getHp() < character.getMaxHp() / 3) {
        return Action::DEFEND;
    }
    // if (defender.getHp() < defender.getMaxHp() / 4) {
    //     return Action::SPECIAL;
    // }
    return Action::ATTACK;
}
