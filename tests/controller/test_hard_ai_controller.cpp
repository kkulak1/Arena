#include <catch2/catch_test_macros.hpp>

#include "controller/HardAIController.h"
#include "character/TestCharacter.h"

TEST_CASE("HardAIController: gdy enemyHP <= selfAttack wybiera ATTACK", "[controller][ai][hard]") {
	HardAIController c;

	TestCharacter self("Self", 1, 100, 20, 0, 0.0, 0.0);
	TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

	enemy.setHealth(20);
	REQUIRE(c.chooseAction(self, enemy) == EAction::ATTACK);

	enemy.setHealth(19);
	REQUIRE(c.chooseAction(self, enemy) == EAction::ATTACK);
}

TEST_CASE("HardAIController: gdy enemyHP > selfAttack zwraca poprawna akcje", "[controller][ai][hard]") {
	HardAIController c;

	TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
	TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

	enemy.setHealth(50);
	auto action = c.chooseAction(self, enemy);
	REQUIRE((action == EAction::ATTACK || action == EAction::DEFEND || action == EAction::SPECIAL));
}

TEST_CASE("HardAIController: priorytet dobijania wygrywa nawet przy niskim self HP", "[controller][ai][hard]") {
	HardAIController c;

	TestCharacter self("Self", 1, 100, 20, 0, 0.0, 0.0);
	self.setHealth(10);
	TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);
	enemy.setHealth(20);

	REQUIRE(c.chooseAction(self, enemy) == EAction::ATTACK);
}

TEST_CASE("HardAIController: przy niskim self HP i bez dobijania preferuje DEFEND", "[controller][ai][hard]") {
	HardAIController c;

	TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
	self.setHealth(20);
	self.configureSpecialCooldown(2);
	self.startSpecialCooldown();
	TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);
	enemy.setHealth(80);

	REQUIRE(c.chooseAction(self, enemy) == EAction::DEFEND);
}


