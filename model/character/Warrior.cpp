//
// Created by kamil on 16.03.2026.
//

#include "../../include/character/Warrior.h"
#include <iostream>

Warrior::Warrior(std::string name)
    : Character(name, 120, 18, 10, 0.15, 0.05) {}

void Warrior::specialAbility(Character &target) {
    int damage = getAttack() * 2;

    std::cout << getName() << " uses POWER STRIKE!" << std::endl;

    target.takeDamage(damage);

    std::cout << target.getName() << " gets damage " << std::endl;
}
