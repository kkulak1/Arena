#include <catch2/catch_test_macros.hpp>

#include "controller/AIController.h"
#include "TestCharacter.h"

namespace {
class TestAIController final : public AIController {
protected:
    int getAttackWeight() const override { return 10; }
    int getDefendWeight() const override { return 10; }
    int getSpecialActionWeight() const override { return 10; }
};
}

TEST_CASE("AIController::decideTurn zwraca decyzje z chooseAction", "[controller][ai]") {
    TestAIController c;

    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::NONE);
    REQUIRE((d.action == EAction::ATTACK || d.action == EAction::DEFEND || d.action == EAction::SPECIAL));
}

TEST_CASE("AIController::chooseAction preferuje DEFEND gdy selfHP < 0.3", "[controller][ai]") {
    class LowHpDefendAI final : public AIController {
    protected:
        int getAttackWeight() const override { return 5; }
        int getDefendWeight() const override { return 50; }
        int getSpecialActionWeight() const override { return 10; }
    } c;

    TestCharacter self("Self", 1, 20, 10, 0, 0.0, 0.0);   // maxHp=20 => selfHp=1.0, potrzebujemy 0.2
    self.setHealth(5);                                     // 5/20 = 0.25 < 0.3
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    REQUIRE(c.chooseAction(self, enemy) == EAction::DEFEND);
}

TEST_CASE("AIController::chooseAction preferuje ATTACK gdy enemyHP < 0.25", "[controller][ai]") {
    class FinishEnemyAI final : public AIController {
    protected:
        int getAttackWeight() const override { return 1; }
        int getDefendWeight() const override { return 40; }
        int getSpecialActionWeight() const override { return 40; }
    } c;

    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);
    enemy.setHealth(10); // 10/100 = 0.1 < 0.25

    REQUIRE(c.chooseAction(self, enemy) == EAction::ATTACK);
}

