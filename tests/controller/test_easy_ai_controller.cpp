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

    REQUIRE(d.command == EGameCommand::NONE);
    REQUIRE((d.action == EAction::ATTACK || d.action == EAction::DEFEND || d.action == EAction::SPECIAL));
}

