#include <catch2/catch_test_macros.hpp>

#include "TestCharacter.h"
#include "character/Mage.h"

TEST_CASE("Mage: konstruktor ustawia statystyki bazowe", "[character][mage]") {
    Mage m("M");

    REQUIRE(m.getCharacterType() == ECharacterType::MAGE);
    REQUIRE(m.getTeamId() == 1);
    REQUIRE(m.getHp() == 80);
    REQUIRE(m.getMaxHp() == 80);
    REQUIRE(m.getAttack() == 22);
    REQUIRE(m.getDefense() == 5);
}

TEST_CASE("Mage::specialAbility zadaje attack + (defense/2) celu", "[character][mage]") {
    Mage m("M");
    // target defense = 10 -> reducedDefense = 5
    TestCharacter target("T", 2, 100, 1, 10, 0.0, 0.0);

    m.specialAbility(target);
    REQUIRE(target.getHp() == 100 - (m.getAttack() + (10 / 2)));
}

