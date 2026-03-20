//
// Created by kamil on 16.03.2026.
//

#include  "../../include/character/Mage.h"
#include <iostream>

Mage::Mage(std::string name)
    : Character(name, 80, 22, 5, 0.15, 0.15) {}

void Mage::specialAbility(Character &target) {
    std::cout << getName() << " casts FIREBALL!" << std::endl;
    int reducedDefense = target.getDefense() / 2;
    int damage = getAttack() + reducedDefense;

    target.takeDamage(damage);

    std::cout << getName() << " takes " << damage << " damage with FIREBALL!" << std::endl;
}
