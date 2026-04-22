//
// Created by kamil on 16.03.2026.
//

#include "../../include/character/Warrior.h"

#include <utility>

Warrior::Warrior(std::string name)
    : Character(std::move(name), 1, 120, 18, 10, 0.15, 0.05) {
    setSpecialCooldownTurns(3);
}

ECharacterType Warrior::getCharacterType() const {
    return ECharacterType::WARRIOR;
}

int Warrior::specialAbility(const Character &) {
    return getAttack() * 2;
}
