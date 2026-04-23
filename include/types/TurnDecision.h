//
// Created by kamil on 12.04.2026.
//

#ifndef ARENA_TURNDECISION_H
#define ARENA_TURNDECISION_H

#include "EAction.h"
#include "EGameCommand.h"

struct TurnDecision {
    EGameCommand command = EGameCommand::CONTINUE;
    EAction action = EAction::ATTACK;
};

#endif //ARENA_TURNDECISION_H

