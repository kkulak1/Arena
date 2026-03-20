//
// Created by kamil on 20.03.2026.
//

#ifndef ARENA_HUMANCONTROLLER_H
#define ARENA_HUMANCONTROLLER_H
#include "Controller.h"
#include "../Action.h"


class HumanController : public Controller {
public:
    Action chooseAction(Character& character) override;
};


#endif //ARENA_HUMANCONTROLLER_H