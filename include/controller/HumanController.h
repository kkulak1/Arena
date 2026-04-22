//
// Created by kamil on 20.03.2026.
//

#ifndef ARENA_HUMANCONTROLLER_H
#define ARENA_HUMANCONTROLLER_H
#include "Controller.h"


class HumanController : public Controller {
public:
    TurnDecision decideTurn(Character& self, Character& enemy) override;
};


#endif //ARENA_HUMANCONTROLLER_H