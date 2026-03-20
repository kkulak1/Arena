//
// Created by kamil on 16.03.2026.
//

#ifndef ARENA_WARRIOR_H
#define ARENA_WARRIOR_H
#include "Character.h"

class Warrior : public Character {
public:
    Warrior(std::string name);

    void specialAbility(Character& target) override;
};

#endif //ARENA_WARRIOR_H