//
// Created by kamil on 16.03.2026.
//

#ifndef ARENA_MAGE_H
#define ARENA_MAGE_H
#include "Character.h"

class Mage : public Character {
public:
    Mage(std::string name);

    void specialAbility(Character &target) override;
};

#endif //ARENA_MAGE_H