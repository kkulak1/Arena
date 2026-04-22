#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "Arena.h"
#include "character/TestCharacter.h"

namespace {
class ScopedCurrentPath {
public:
    ScopedCurrentPath() {
        originalPath_ = std::filesystem::current_path();
        const auto stamp = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
        tempPath_ = std::filesystem::temp_directory_path() / ("arena_arena_tests_" + stamp);
        std::filesystem::create_directories(tempPath_);
        std::filesystem::current_path(tempPath_);
    }

    ~ScopedCurrentPath() {
        std::filesystem::current_path(originalPath_);
        std::error_code ec;
        std::filesystem::remove_all(tempPath_, ec);
    }

private:
    std::filesystem::path originalPath_;
    std::filesystem::path tempPath_;
};

class ScopedCin {
public:
    explicit ScopedCin(const std::string& input) : input_(input) {
        old_ = std::cin.rdbuf(input_.rdbuf());
    }

    ~ScopedCin() { std::cin.rdbuf(old_); }

private:
    std::istringstream input_;
    std::streambuf* old_ = nullptr;
};

class ScopedCout {
public:
    ScopedCout() { old_ = std::cout.rdbuf(output_.rdbuf()); }

    ~ScopedCout() { std::cout.rdbuf(old_); }

    std::string str() const { return output_.str(); }

private:
    std::ostringstream output_;
    std::streambuf* old_ = nullptr;
};

class FixedController final : public Controller {
public:
    explicit FixedController(TurnDecision decision) : decision_(decision) {}

    TurnDecision decideTurn(Character&, Character&) override {
        return decision_;
    }

private:
    TurnDecision decision_;
};

class CountingController final : public Controller {
public:
    explicit CountingController(TurnDecision decision) : decision_(decision) {}

    TurnDecision decideTurn(Character&, Character&) override {
        ++calls_;
        return decision_;
    }

    int calls() const { return calls_; }

private:
    TurnDecision decision_;
    int calls_ = 0;
};

class SpecialTestCharacter final : public Character {
public:
    SpecialTestCharacter(std::string name, int teamId, int hp, int attack, int defense)
        : Character(std::move(name), teamId, hp, attack, defense, 0.0, 0.0) {
        setSpecialCooldownTurns(2);
    }

    int specialAbility(const Character&) override {
        ++specialCalls_;
        return 7;
    }

    int getSpecialCalls() const { return specialCalls_; }

    ECharacterType getCharacterType() const override { return ECharacterType::WARRIOR; }

private:
    int specialCalls_ = 0;
};
} // namespace

TEST_CASE("Arena::executeAction DEFEND ustawia defending", "[arena]") {
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    TestCharacter attacker("A", 1, 100, 10, 1, 0.0, 0.0);
    TestCharacter defender("D", 2, 100, 10, 1, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(EAction::DEFEND, attacker, defender);

    REQUIRE(attacker.isDefending());
}

TEST_CASE("Arena::executeAction SPECIAL wywoluje specialAbility", "[arena]") {
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    SpecialTestCharacter attacker("A", 1, 100, 10, 1);
    TestCharacter defender("D", 2, 100, 10, 1, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(EAction::SPECIAL, attacker, defender);

    REQUIRE(attacker.getSpecialCalls() == 1);
    REQUIRE(defender.getHp() == 94);
}

TEST_CASE("Arena::executeAction SPECIAL na cooldownie przechodzi na ATTACK", "[arena]") {
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    SpecialTestCharacter attacker("A", 1, 100, 10, 1);
    TestCharacter defender("D", 2, 100, 10, 1, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(EAction::SPECIAL, attacker, defender);
    arena.executeAction(EAction::SPECIAL, attacker, defender);

    REQUIRE(attacker.getSpecialCalls() == 1);
    REQUIRE(defender.getHp() == 85);
}

TEST_CASE("Arena::executeAction SPECIAL uwzglednia defense i aktywna obrone", "[arena]") {
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    SpecialTestCharacter attacker("A", 1, 100, 10, 1);
    TestCharacter defender("D", 2, 100, 10, 3, 0.0, 0.0);
    defender.defend();

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(EAction::SPECIAL, attacker, defender);

    // 7 (special) - 3 (defense) = 4, a przy defend obrazenia sa polowione.
    REQUIRE(defender.getHp() == 98);
    REQUIRE_FALSE(defender.isDefending());
}

TEST_CASE("Arena::executeAction SPECIAL po redukcji defense moze zadac 0 obrazen", "[arena]") {
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    SpecialTestCharacter attacker("A", 1, 100, 10, 1);
    TestCharacter defender("D", 2, 100, 10, 20, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(EAction::SPECIAL, attacker, defender);

    REQUIRE(defender.getHp() == 100);
    REQUIRE(attacker.getSpecialCalls() == 1);
}

TEST_CASE("Arena::executeAction nieznana akcja domyslnie ATTACK", "[arena]") {
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    TestCharacter attacker("A", 1, 100, 10, 1, 0.0, 0.0);
    TestCharacter defender("D", 2, 100, 10, 3, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(static_cast<EAction>(999), attacker, defender);

    REQUIRE(defender.getHp() == 93);
}

TEST_CASE("Arena::startGame zapisuje i konczy po SAVE_AND_EXIT", "[arena]") {
    ScopedCurrentPath cwd;
    ScopedCin input("arena_slot\n");
    ScopedCout captured;

    TurnDecision saveDecision;
    saveDecision.command = EGameCommand::SAVE_AND_EXIT;

    TurnDecision attackDecision;
    attackDecision.action = EAction::ATTACK;

    FixedController c1(saveDecision);
    FixedController c2(attackDecision);

    TestCharacter p1("P1", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter p2("P2", 2, 100, 10, 0, 0.0, 0.0);

    Arena arena(&p1, &p2, &c1, &c2, 5, 2, 1);
    arena.startGame();

    const auto savePath = std::filesystem::current_path() / "saves" / "arena_slot.txt";
    REQUIRE(std::filesystem::exists(savePath));

    std::ifstream in(savePath);
    REQUIRE(in.is_open());

    std::string header;
    std::getline(in, header);
    REQUIRE(header == "5 2 1");

    REQUIRE(captured.str().find("Game saved. Exiting...") != std::string::npos);
}

TEST_CASE("Arena::executeAction ATTACK zadaje poprawne obrazenia", "[arena]") {
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    TestCharacter attacker("A", 1, 100, 12, 1, 0.0, 0.0);
    TestCharacter defender("D", 2, 100, 10, 2, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(EAction::ATTACK, attacker, defender);

    REQUIRE(defender.getHp() == 90);
}

TEST_CASE("Arena::executeAction ATTACK po uniku zdejmuje flage defend", "[arena]") {
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    TestCharacter attacker("A", 1, 100, 12, 1, 0.0, 0.0);
    TestCharacter defender("D", 2, 100, 10, 2, 0.0, 1.0);
    defender.defend();

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(EAction::ATTACK, attacker, defender);

    REQUIRE(defender.getHp() == 100);
    REQUIRE_FALSE(defender.isDefending());
}

TEST_CASE("Arena::executeAction DEFEND wypisuje komunikat", "[arena]") {
    ScopedCout captured;
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    TestCharacter attacker("Shield", 1, 100, 10, 1, 0.0, 0.0);
    TestCharacter defender("Enemy", 2, 100, 10, 1, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(EAction::DEFEND, attacker, defender);

    REQUIRE(captured.str().find("Shield is defending this turn.") != std::string::npos);
}

TEST_CASE("Arena::executeAction unknown action wypisuje fallback", "[arena]") {
    ScopedCout captured;
    TurnDecision attackDecision{};
    FixedController c1(attackDecision);
    FixedController c2(attackDecision);

    TestCharacter attacker("A", 1, 100, 10, 1, 0.0, 0.0);
    TestCharacter defender("D", 2, 100, 10, 3, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeAction(static_cast<EAction>(999), attacker, defender);

    REQUIRE(captured.str().find("Unknown action, defaulting to ATTACK.") != std::string::npos);
}

TEST_CASE("Arena::executeTurn wykonuje akcje zwrocona przez controller", "[arena]") {
    TurnDecision defendDecision;
    defendDecision.action = EAction::DEFEND;

    FixedController c1(defendDecision);
    FixedController c2(defendDecision);
    TestCharacter attacker("A", 1, 100, 10, 1, 0.0, 0.0);
    TestCharacter defender("D", 2, 100, 10, 1, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2);
    arena.executeTurn(attacker, defender, &c1);

    REQUIRE(attacker.isDefending());
}

TEST_CASE("Arena::executeTurn SAVE_AND_EXIT ma priorytet nad akcja", "[arena]") {
    ScopedCurrentPath cwd;
    ScopedCin input("turn_save\n");

    TurnDecision saveAndAttackDecision;
    saveAndAttackDecision.command = EGameCommand::SAVE_AND_EXIT;
    saveAndAttackDecision.action = EAction::ATTACK;

    FixedController c1(saveAndAttackDecision);
    FixedController c2(saveAndAttackDecision);
    TestCharacter attacker("A", 1, 100, 30, 1, 0.0, 0.0);
    TestCharacter defender("D", 2, 100, 10, 0, 0.0, 0.0);

    Arena arena(&attacker, &defender, &c1, &c2, 3, 2, 1);
    arena.executeTurn(attacker, defender, &c1);

    REQUIRE(defender.getHp() == 100);
    REQUIRE(std::filesystem::exists(std::filesystem::path("saves") / "turn_save.txt"));
}

TEST_CASE("Arena::startGame oglasza zwyciezce gdy gracz jest martwy na starcie", "[arena]") {
    ScopedCout captured;

    TurnDecision attackDecision;
    attackDecision.action = EAction::ATTACK;

    CountingController c1(attackDecision);
    CountingController c2(attackDecision);
    TestCharacter p1("P1", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter p2("P2", 2, 100, 10, 0, 0.0, 0.0);
    p1.setHealth(0);

    Arena arena(&p1, &p2, &c1, &c2);
    arena.startGame();

    REQUIRE(c1.calls() == 0);
    REQUIRE(c2.calls() == 0);
    REQUIRE(captured.str().find("Game Over!") != std::string::npos);
    REQUIRE(captured.str().find("P2 wins!") != std::string::npos);
}

TEST_CASE("Arena::startGame pomija controller2 gdy player1 nokautuje od razu", "[arena]") {
    ScopedCout captured;

    TurnDecision attackDecision;
    attackDecision.action = EAction::ATTACK;

    CountingController c1(attackDecision);
    CountingController c2(attackDecision);
    TestCharacter p1("P1", 1, 100, 200, 0, 0.0, 0.0);
    TestCharacter p2("P2", 2, 100, 10, 0, 0.0, 0.0);

    Arena arena(&p1, &p2, &c1, &c2);
    arena.startGame();

    REQUIRE(c1.calls() == 1);
    REQUIRE(c2.calls() == 0);
    REQUIRE(captured.str().find("P1 wins!") != std::string::npos);
}

