//
// Created by kamil on 20.03.2026.
//

#ifndef ARENA_HUMANCONTROLLER_H
#define ARENA_HUMANCONTROLLER_H
#include "Controller.h"
#include "../EAction.h"


class HumanController : public Controller {
public:
    EAction chooseAction(Character& self, Character& enemy) override;
};


#endif //ARENA_HUMANCONTROLLER_H