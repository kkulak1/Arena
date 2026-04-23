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

TEST_CASE("Mage::specialAbility leczy postac", "[character][mage]") {
    Mage m("M");
    m.setHealth(40);
    TestCharacter target("T", 2, 100, 1, 10, 0.0, 0.0);

    const int heal = m.specialAbility(target);
    REQUIRE(heal == 30);
    REQUIRE(m.getHp() == 70);
}

TEST_CASE("Mage::specialAbility nie leczy powyzej max HP", "[character][mage]") {
    Mage m("M");
    m.setHealth(70);
    TestCharacter target("T", 2, 100, 1, 9, 0.0, 0.0);

    const int heal = m.specialAbility(target);
    REQUIRE(heal == 10);
    REQUIRE(m.getHp() == 80);
}

TEST_CASE("Mage ma cooldown speciala rowny 3 tury", "[character][mage]") {
    Mage m("M");

    m.startSpecialCooldown();
    REQUIRE(m.getSpecialCooldownRemaining() == 4);
}
