#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include <sstream>

#include "controller/HumanController.h"
#include "character/TestCharacter.h"

namespace {
    struct CinRedirect {
        std::streambuf* oldBuf;
        explicit CinRedirect(std::istream& in, std::streambuf* newBuf)
            : oldBuf(in.rdbuf(newBuf)) {}
        ~CinRedirect() {
            std::cin.rdbuf(oldBuf);
        }
        CinRedirect(const CinRedirect&) = delete;
        CinRedirect& operator=(const CinRedirect&) = delete;
    };

    struct CoutRedirect {
        std::streambuf* oldBuf;
        std::ostringstream output;

        CoutRedirect() : oldBuf(std::cout.rdbuf(output.rdbuf())) {}
        ~CoutRedirect() {
            std::cout.rdbuf(oldBuf);
        }

        std::string str() const { return output.str(); }

        CoutRedirect(const CoutRedirect&) = delete;
        CoutRedirect& operator=(const CoutRedirect&) = delete;
    };
}

TEST_CASE("HumanController: wybiera ATTACK gdy uzytkownik wpisze 1", "[controller][human]") {
    std::istringstream in("1\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::CONTINUE);
    REQUIRE(d.action == EAction::ATTACK);
}

TEST_CASE("HumanController: wybiera DEFEND gdy uzytkownik wpisze 2", "[controller][human]") {
    std::istringstream in("2\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::CONTINUE);
    REQUIRE(d.action == EAction::DEFEND);
}

TEST_CASE("HumanController: wybiera SPECIAL gdy uzytkownik wpisze 3", "[controller][human]") {
    std::istringstream in("3\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::CONTINUE);
    REQUIRE(d.action == EAction::SPECIAL);
}

TEST_CASE("HumanController: wybiera SAVE_AND_EXIT gdy uzytkownik wpisze 4", "[controller][human]") {
    std::istringstream in("4\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::SAVE_AND_EXIT);
}

TEST_CASE("HumanController: gdy SPECIAL jest na cooldownie, wybor 3 daje ATTACK", "[controller][human]") {
    std::istringstream in("3\n");
    CinRedirect redirect(std::cin, in.rdbuf());
    CoutRedirect out;

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    self.configureSpecialCooldown(2);
    self.startSpecialCooldown();
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::CONTINUE);
    REQUIRE(d.action == EAction::ATTACK);
    REQUIRE(out.str().find("Special ability is on cooldown") != std::string::npos);
}

TEST_CASE("HumanController: niepoprawny input domyslnie ATTACK", "[controller][human]") {
    std::istringstream in("999\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);


    REQUIRE(d.command == EGameCommand::CONTINUE);
    REQUIRE(d.action == EAction::ATTACK);
}

TEST_CASE("HumanController: litera nie przeskakuje tury, tylko ponawia wybor", "[controller][human]") {
    std::istringstream in("a\n2\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::CONTINUE);
    REQUIRE(d.action == EAction::DEFEND);
}

TEST_CASE("HumanController: kilka blednych wejsc pod rzad konczy sie poprawna akcja", "[controller][human]") {
    std::istringstream in("x\nabc\n4\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::SAVE_AND_EXIT);
}

TEST_CASE("HumanController: EOF domyslnie wybiera ATTACK", "[controller][human]") {
    std::istringstream in("");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::CONTINUE);
    REQUIRE(d.action == EAction::ATTACK);
}

TEST_CASE("HumanController: liczba z ogonem tekstowym nadal czyta liczbe", "[controller][human]") {
    std::istringstream in("2abc\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::CONTINUE);
    REQUIRE(d.action == EAction::DEFEND);
}

