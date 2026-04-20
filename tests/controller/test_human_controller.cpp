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
}

TEST_CASE("HumanController: wybiera ATTACK gdy uzytkownik wpisze 1", "[controller][human]") {
    std::istringstream in("1\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::NONE);
    REQUIRE(d.action == EAction::ATTACK);
}

TEST_CASE("HumanController: wybiera DEFEND gdy uzytkownik wpisze 2", "[controller][human]") {
    std::istringstream in("2\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::NONE);
    REQUIRE(d.action == EAction::DEFEND);
}

TEST_CASE("HumanController: wybiera SPECIAL gdy uzytkownik wpisze 3", "[controller][human]") {
    std::istringstream in("3\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);

    REQUIRE(d.command == EGameCommand::NONE);
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

TEST_CASE("HumanController: niepoprawny input domyslnie ATTACK", "[controller][human]") {
    std::istringstream in("999\n");
    CinRedirect redirect(std::cin, in.rdbuf());

    HumanController c;
    TestCharacter self("Self", 1, 100, 10, 0, 0.0, 0.0);
    TestCharacter enemy("Enemy", 2, 100, 10, 0, 0.0, 0.0);

    TurnDecision d = c.decideTurn(self, enemy);


    REQUIRE(d.command == EGameCommand::NONE);
    REQUIRE(d.action == EAction::ATTACK);
}

