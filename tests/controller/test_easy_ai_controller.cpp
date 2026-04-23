#include <catch2/catch_test_macros.hpp>

#include <cstdlib>

#include "controller/EasyAIController.h"
#include "character/TestCharacter.h"

TEST_CASE("EasyAIController: decideTurn zwraca jedna z akcji (ATTACK/DEFEND/SPECIAL)", "[controller][ai][easy]") {
    std::srand(0);

    EasyAIController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::CONTINUE);
    REQUIRE((d.action == EAction::ATTACK || d.action == EAction::DEFEND || d.action == EAction::SPECIAL));
}

TEST_CASE("EasyAIController: w serii tur pojawiaja sie rozne akcje", "[controller][ai][easy]") {
    std::srand(1);

    EasyAIController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    bool seenAttack = false;
    bool seenDefend = false;
    bool seenSpecial = false;

    for (int i = 0; i < 200; ++i) {
        const EAction action = c.chooseAction(self, enemy);
        seenAttack = seenAttack || action == EAction::ATTACK;
        seenDefend = seenDefend || action == EAction::DEFEND;
        seenSpecial = seenSpecial || action == EAction::SPECIAL;
    }

    REQUIRE(seenAttack);
    REQUIRE(seenDefend);
    REQUIRE(seenSpecial);
}

TEST_CASE("EasyAIController: przy cooldownie special nadal moze byc losowo wybrany", "[controller][ai][easy]") {
    std::srand(2);

    EasyAIController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    self.configureSpecialCooldown(2);
    self.startSpecialCooldown();
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    bool pickedSpecial = false;
    for (int i = 0; i < 250; ++i) {
        if (c.chooseAction(self, enemy) == EAction::SPECIAL) {
            pickedSpecial = true;
            break;
        }
    }

    REQUIRE(pickedSpecial);
}

