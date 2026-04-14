//
// Created by kamil on 20.03.2026.
//

#ifndef ARENA_CONTROLLER_H
#define ARENA_CONTROLLER_H
#include "../types/TurnDecision.h"
#include "../character/Character.h"


class Controller {
public:
    virtual TurnDecision decideTurn(Character& self, Character& enemy) = 0; // =0 bo musi byc nadpisana w klasie pochodnej
    virtual ~Controller() = default;
};


#endif //ARENA_CONTROLLER_H