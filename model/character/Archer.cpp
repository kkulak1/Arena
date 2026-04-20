//
// Created by kamil on 16.03.2026.
//

#include "../../include/character/Archer.h"
#include "../../include/ConsoleRenderer.h"
#include "../../include/types/Color.h"

#include <utility>

Archer::Archer(std::string name)
    : Character(std::move(name), 1, 90, 16, 6, 0.25, 0.20) {}

ECharacterType Archer::getCharacterType() const {
    return ECharacterType::ARCHER;
}

void Archer::specialAbility(Character &target) {
    ConsoleRenderer::printMessage(getName() + " uses DOUBLE SHOT!" , Color::Default, this);
    ConsoleRenderer::printMessage(getName() + " shoots the first arrow!", Color::Default, this);
    attackTarget(target);
    ConsoleRenderer::printMessage(getName() + " shoots the second arrow!", Color::Default, this);
    attackTarget(target);
}
