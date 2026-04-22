//
// Created by kamil on 23.04.2026.
//

#ifndef ARENA_DAMAGECALCULATOR_H
#define ARENA_DAMAGECALCULATOR_H

#include <algorithm>

#include "character/Character.h"

template<typename Modifier>
int calculatedDamageFromBase(int base, const Character& attacker, const Character& defender, Modifier modifier) {
    const int modified = modifier(base, attacker, defender);
    return std::max(0, modified);
}

template<typename Modifier>
int calculatedDamage(const Character& attacker, const Character& defender, Modifier modifier) {
    return calculatedDamageFromBase(attacker.getAttack() - defender.getDefense(), attacker, defender, modifier);
}

#endif //ARENA_DAMAGECALCULATOR_H

