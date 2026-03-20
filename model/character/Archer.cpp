//
// Created by kamil on 16.03.2026.
//

#include "../../include/character/Archer.h"
#include <iostream>

Archer::Archer(std::string name)
    : Character(name, 90, 16, 6, 0.25, 0.20) {}

void Archer::specialAbility(Character &target) {
    std::cout << getName() << " uses DOUBLE SHOT!" << std::endl;

    std::cout << getName() << " shoots the first arrow!" << std::endl;
    attackTarget(target);
    std::cout << getName() << " shoots the second arrow!" << std::endl;
    attackTarget(target);
}
