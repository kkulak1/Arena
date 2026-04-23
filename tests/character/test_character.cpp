#include <catch2/catch_test_macros.hpp>

#include "TestCharacter.h"
#include "character/Character.h"

TEST_CASE("Character::defend ustawia defending", "[character]") {
    TestCharacter c("A");
    REQUIRE_FALSE(c.isDefending());

    c.defend();
    REQUIRE(c.isDefending());
}

TEST_CASE("Character::takeDamage zmniejsza HP i nie schodzi ponizej 0", "[character]") {
    TestCharacter c("A", 1, 10);
    c.takeDamage(3);
    REQUIRE(c.getHp() == 7);

    c.takeDamage(999);
    REQUIRE(c.getHp() == 0);
    REQUIRE_FALSE(c.isAlive());
}

TEST_CASE("Character::getMaxHp to wartosc poczatkowa HP", "[character]") {
    TestCharacter c("A", 1, 123);
    REQUIRE(c.getHp() == 123);
    REQUIRE(c.getMaxHp() == 123);

    c.takeDamage(50);
    REQUIRE(c.getHp() == 73);
    REQUIRE(c.getMaxHp() == 123);
}

TEST_CASE("Character::defend redukuje pierwsze otrzymane obrazenia", "[character]") {
    TestCharacter c("A", 1, 100);

    c.defend();
    c.takeDamage(9);
    REQUIRE(c.getHp() == 91);

    c.takeDamage(9);
    REQUIRE(c.getHp() == 82);
}

TEST_CASE("Character cooldown speciala odlicza sie poprawnie", "[character]") {
    TestCharacter c("A");
    c.configureSpecialCooldown(2);

    REQUIRE(c.canUseSpecial());
    REQUIRE(c.getSpecialCooldownRemaining() == 0);

    c.startSpecialCooldown();
    REQUIRE_FALSE(c.canUseSpecial());
    REQUIRE(c.getSpecialCooldownRemaining() == 3);

    c.tickSpecialCooldown();
    REQUIRE(c.getSpecialCooldownRemaining() == 2);
    REQUIRE_FALSE(c.canUseSpecial());

    c.tickSpecialCooldown();
    REQUIRE(c.getSpecialCooldownRemaining() == 1);
    REQUIRE_FALSE(c.canUseSpecial());

    c.tickSpecialCooldown();
    REQUIRE(c.getSpecialCooldownRemaining() == 0);
    REQUIRE(c.canUseSpecial());
}


