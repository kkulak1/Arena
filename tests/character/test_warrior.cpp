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
