//
// Created by kamil on 16.03.2026.
//

#include  "../../include/character/Mage.h"

#include <utility>

Mage::Mage(std::string name)
    : Character(std::move(name), 1, 80, 22, 5, 0.15, 0.15) {
    setSpecialCooldownTurns(3);
}

ECharacterType Mage::getCharacterType() const {
    return ECharacterType::MAGE;
}

int Mage::specialAbility(const Character &target) {
    int healAmount = 30; // base heal amount
    int previousHp = getHp();
    int newHp = previousHp + healAmount;
    if (newHp > getMaxHp()) {
        newHp = getMaxHp();
    }
    setHealth(newHp);
    return newHp - previousHp;
}
