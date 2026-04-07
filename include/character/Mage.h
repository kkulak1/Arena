//
// Created by kamil on 16.03.2026.
//

#ifndef ARENA_MAGE_H
#define ARENA_MAGE_H
#include "Character.h"
#include "../ECharacterType.h"

class Mage : public Character {
public:
    Mage(std::string name);
    ECharacterType getCharacterType() const override;
    void specialAbility(Character &target) override;
};

#endif //ARENA_MAGE_H