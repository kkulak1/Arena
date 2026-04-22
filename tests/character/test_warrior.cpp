#include <catch2/catch_test_macros.hpp>

#include "TestCharacter.h"
#include "character/Warrior.h"

TEST_CASE("Warrior: konstruktor ustawia statystyki bazowe", "[character][warrior]") {
    Warrior w("W");

    REQUIRE(w.getCharacterType() == ECharacterType::WARRIOR);
    REQUIRE(w.getTeamId() == 1);
    REQUIRE(w.getHp() == 120);
    REQUIRE(w.getMaxHp() == 120);
    REQUIRE(w.getAttack() == 18);
    REQUIRE(w.getDefense() == 10);
}

TEST_CASE("Warrior::specialAbility zadaje podwojony attack", "[character][warrior]") {
    Warrior w("W");
    TestCharacter target("T", 2, 100, 10, 999, 0.0, 0.0);

    REQUIRE(w.specialAbility(target) == w.getAttack() * 2);
}

TEST_CASE("Warrior ma cooldown speciala rowny 3 tury", "[character][warrior]") {
    Warrior w("W");

    w.startSpecialCooldown();
    REQUIRE(w.getSpecialCooldownRemaining() == 3);

    w.tickSpecialCooldown();
    REQUIRE(w.getSpecialCooldownRemaining() == 2);
}

