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

TEST_CASE("Archer::specialAbility to podwojone obrazenia pojedynczego strzalu", "[character][archer]") {
    Archer a("A");
    TestCharacter target("T", 2, 100, 1, 6, 0.0, 0.0);

    const int singleShotDamage = a.getAttack() - target.getDefense();
    REQUIRE(a.specialAbility(target) == singleShotDamage * 2 + target.getDefense());
}

TEST_CASE("Archer::specialAbility nie schodzi ponizej zera", "[character][archer]") {
    Archer a("A");
    TestCharacter target("T", 2, 100, 1, 100, 0.0, 0.0);

    REQUIRE(a.specialAbility(target) == target.getDefense());
}

TEST_CASE("Archer ma cooldown speciala rowny 2 tury", "[character][archer]") {
    Archer a("A");

    a.startSpecialCooldown();
    REQUIRE(a.getSpecialCooldownRemaining() == 2);
}
