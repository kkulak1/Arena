//
// Created by kamil on 16.03.2026.
//

#include "../../include/character/Archer.h"
#include "../../include/templates/MathClamp.h"

#include <utility>

Archer::Archer(std::string name)
    : Character(std::move(name), 1, 90, 16, 6, 0.25, 0.20) {
    setSpecialCooldownTurns(3);
}

ECharacterType Archer::getCharacterType() const {
    return ECharacterType::ARCHER;
}

int Archer::specialAbility(const Character &target) {
    const int singleShotDamage = clampMin(getAttack() - target.getDefense(), 0);
    return singleShotDamage * 2 + target.getDefense();
}
