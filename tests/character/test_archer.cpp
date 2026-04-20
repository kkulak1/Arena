#include <catch2/catch_test_macros.hpp>

#include <cstdlib>

#include "TestCharacter.h"
#include "character/Archer.h"

TEST_CASE("Archer: konstruktor ustawia statystyki bazowe", "[character][archer]") {
    Archer a("A");

    REQUIRE(a.getCharacterType() == ECharacterType::ARCHER);
    REQUIRE(a.getTeamId() == 1);
    REQUIRE(a.getHp() == 90);
    REQUIRE(a.getMaxHp() == 90);
    REQUIRE(a.getAttack() == 16);
    REQUIRE(a.getDefense() == 6);
}
