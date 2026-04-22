#ifndef ARENA_TEST_CHARACTER_H
#define ARENA_TEST_CHARACTER_H

#include <string>
#include <utility>

#include "character/Character.h"

// Prosta implementacja Character do testów (Character jest abstrakcyjny).
class TestCharacter final : public Character {
public:
    explicit TestCharacter(std::string name,
                           int teamId = 1,
                           int hp = 100,
                           int attack = 10,
                           int defense = 2,
                           double critChance = 0.0,
                           double dodgeChance = 0.0)
        : Character(std::move(name), teamId, hp, attack, defense, critChance, dodgeChance) {}

    int specialAbility(const Character&) override { return 0; }

    void configureSpecialCooldown(int turns) { setSpecialCooldownTurns(turns); }

    [[nodiscard]] ECharacterType getCharacterType() const override {
        return ECharacterType::WARRIOR;
    }
};

#endif // ARENA_TEST_CHARACTER_H

