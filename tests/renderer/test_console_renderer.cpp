#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include "ConsoleRenderer.h"
#include "character/TestCharacter.h"
#include "types/Color.h"

namespace {
class ScopedCout {
public:
    ScopedCout() { old_ = std::cout.rdbuf(output_.rdbuf()); }

    ~ScopedCout() { std::cout.rdbuf(old_); }

    std::string str() const { return output_.str(); }

private:
    std::ostringstream output_;
    std::streambuf* old_ = nullptr;
};
} // namespace

TEST_CASE("ConsoleRenderer::printMessage drukuje kolor, wiadomosc i reset", "[renderer]") {
    ScopedCout captured;

    ConsoleRenderer::printMessage("Hello", Color::Red);

    REQUIRE(captured.str() == "\033[31mHello\n\033[0m");
}

TEST_CASE("ConsoleRenderer::printMessage z player uzywa koloru druzyny", "[renderer]") {
    ScopedCout captured;
    TestCharacter player("BlueGuy", 2);

    ConsoleRenderer::printMessage("Team message", Color::Red, &player);

    REQUIRE(captured.str() == "\033[34mTeam message\n\033[0m");
}

TEST_CASE("ConsoleRenderer::printMessage z nullptr zachowuje podany kolor", "[renderer]") {
    ScopedCout captured;

    ConsoleRenderer::printMessage("No player", Color::Blue, nullptr);

    REQUIRE(captured.str() == "\033[34mNo player\n\033[0m");
}

TEST_CASE("ConsoleRenderer::printPlayer drukuje nazwe i hp", "[renderer]") {
    ScopedCout captured;
    TestCharacter player("P1", 1, 120, 10, 2, 0.0, 0.0);

    ConsoleRenderer::printPlayer(player);

    REQUIRE(captured.str() == "\033[31mP1 - HP: 120/120\n\033[0m");
}

