//
// Created by kamil on 16.03.2026.
//

#ifndef ARENA_ARCHER_H
#define ARENA_ARCHER_H
#include "Character.h"

class Archer : public Character {
public:
    Archer(std::string name);
    ECharacterType getCharacterType() const override;
    int specialAbility(const Character &target) override;
};

#endif //ARENA_ARCHER_H