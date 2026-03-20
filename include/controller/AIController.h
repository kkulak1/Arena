//
// Created by kamil on 16.03.2026.
//

#ifndef ARENA_AICONTROLLER_H
#define ARENA_AICONTROLLER_H
#include "Controller.h"
#include "../Action.h"
#include "../character/Character.h"

class AIController : public Controller{
public:
    Action chooseAction(Character& character) override;
};

#endif //ARENA_AICONTROLLER_H