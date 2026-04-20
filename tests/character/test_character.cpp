#include <catch2/catch_test_macros.hpp>

#include <cstdlib>
#include <ctime>

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

TEST_CASE("Character::attackTarget nie zadaje ujemnych obrazen (attack < defense)", "[character]") {
    TestCharacter attacker("Att", 1, 100, /*attack*/ 1, /*defense*/ 0, /*crit*/ 0.0, /*dodge*/ 0.0);
    TestCharacter target("Tgt", 2, 100, /*attack*/ 10, /*defense*/ 99, /*crit*/ 0.0, /*dodge*/ 0.0);

    attacker.attackTarget(target);
    REQUIRE(target.getHp() == 100);
}

TEST_CASE("Character::attackTarget zadaje obrazenia = attack - defense (bez crit/dodge)", "[character]") {

    TestCharacter attacker("Att", 1, 100, /*attack*/ 10, /*defense*/ 0, /*crit*/ 0.0, /*dodge*/ 0.0);
    TestCharacter target("Tgt", 2, 100, /*attack*/ 10, /*defense*/ 3, /*crit*/ 0.0, /*dodge*/ 0.0);

    attacker.attackTarget(target);
    REQUIRE(target.getHp() == 93);
}


