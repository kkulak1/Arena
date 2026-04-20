//
// Created by kamil on 16.03.2026.
//

#include  "../../include/character/Mage.h"
#include "../../include/ConsoleRenderer.h"
#include "../../include/types/Color.h"

#include <utility>

Mage::Mage(std::string name)
    : Character(std::move(name), 1, 80, 22, 5, 0.15, 0.15) {}

ECharacterType Mage::getCharacterType() const {
    return ECharacterType::MAGE;
}

void Mage::specialAbility(Character &target) {
    ConsoleRenderer::printMessage(getName() + " casts FIREBALL!", Color::Default, this);
    int reducedDefense = target.getDefense() / 2;
    int damage = getAttack() + reducedDefense;

    target.takeDamage(damage);

    ConsoleRenderer::printMessage(getName() + " takes " + std::to_string(damage) + " damage with FIREBALL!", Color::Default, this);
}
