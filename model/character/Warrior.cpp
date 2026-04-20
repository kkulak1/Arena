//
// Created by kamil on 16.03.2026.
//

#include "../../include/character/Warrior.h"
#include "../../include/ConsoleRenderer.h"
#include "../../include/types/Color.h"

#include <utility>

Warrior::Warrior(std::string name)
    : Character(std::move(name), 1, 120, 18, 10, 0.15, 0.05) {}

ECharacterType Warrior::getCharacterType() const {
    return ECharacterType::WARRIOR;
}

void Warrior::specialAbility(Character &target) {
    int damage = getAttack() * 2;

    ConsoleRenderer::printMessage(getName() + " uses POWER STRIKE!", Color::Default, this);

    target.takeDamage(damage);

    ConsoleRenderer::printMessage(target.getName() + " gets damage", Color::Default, &target);
}
