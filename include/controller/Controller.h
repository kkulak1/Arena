//
// Created by kamil on 20.03.2026.
//

#ifndef ARENA_CONTROLLER_H
#define ARENA_CONTROLLER_H
#include "../Action.h"
#include "../character/Character.h"


class Controller {
public:
    virtual Action chooseAction(Character& character) = 0;    // =0 bo musi byc nadpisana w klasie dziedziczacej
    virtual ~Controller() = default;
};


#endif //ARENA_CONTROLLER_H