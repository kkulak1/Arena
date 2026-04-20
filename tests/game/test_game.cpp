#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Game.h"

namespace {
class ScopedCurrentPath {
public:
    ScopedCurrentPath() {
        originalPath_ = std::filesystem::current_path();
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        tempPath_ = std::filesystem::temp_directory_path() / ("arena_game_tests_" + std::to_string(stamp));
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

void writeSaveFile(const std::string& fileName, const std::string& content) {
    std::filesystem::create_directories("saves");
    std::ofstream out(std::filesystem::path("saves") / fileName);
    out << content;
}
} // namespace

TEST_CASE("Game::startGame konczy dla opcji Exit", "[game]") {
    ScopedCin input("3\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    REQUIRE(captured.str().find("Exiting game. Goodbye!") != std::string::npos);
}

TEST_CASE("Game::startGame wypisuje blad gdy load sie nie powiedzie", "[game]") {
    ScopedCurrentPath cwd;
    ScopedCin input("2\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    REQUIRE(captured.str().find("No saves found!") != std::string::npos);
    REQUIRE(captured.str().find("Load failed. Exiting game.") != std::string::npos);
}

TEST_CASE("Game::startGame nowa gra pozwala zapisac i wyjsc", "[game]") {
    ScopedCurrentPath cwd;
    ScopedCin input("1\n1\n1\n1\n4\nquick_slot\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    const auto savePath = std::filesystem::current_path() / "saves" / "quick_slot.txt";
    REQUIRE(std::filesystem::exists(savePath));

    std::ifstream in(savePath);
    REQUIRE(in.is_open());

    std::string header;
    std::getline(in, header);
    REQUIRE(header == "1 1 0");

    REQUIRE(captured.str().find("Game saved. Exiting...") != std::string::npos);
}

TEST_CASE("Game::startGame niepoprawny tryb domyslnie ustawia Player vs AI", "[game]") {
    ScopedCurrentPath cwd;
    ScopedCin input("1\n9\n1\n1\n1\n4\nmode_default\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    const auto savePath = std::filesystem::current_path() / "saves" / "mode_default.txt";
    REQUIRE(std::filesystem::exists(savePath));

    std::ifstream in(savePath);
    std::string header;
    std::getline(in, header);
    REQUIRE(header == "1 2 1");
    REQUIRE(captured.str().find("Invalid choice, defaulting to Player vs AI.") != std::string::npos);
}

TEST_CASE("Game::startGame niepoprawna trudnosc AI domyslnie ustawia Easy", "[game]") {
    ScopedCurrentPath cwd;
    ScopedCin input("1\n2\n1\n1\n9\n4\nai_default\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    const auto savePath = std::filesystem::current_path() / "saves" / "ai_default.txt";
    REQUIRE(std::filesystem::exists(savePath));

    std::ifstream in(savePath);
    std::string header;
    std::getline(in, header);
    REQUIRE(header == "1 2 1");
    REQUIRE(captured.str().find("Invalid choice, defaulting to Easy AI.") != std::string::npos);
}

TEST_CASE("Game::startGame niepoprawna postac domyslnie wybiera Warrior", "[game]") {
    ScopedCurrentPath cwd;
    ScopedCin input("1\n1\n9\n2\n4\nchar_default\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    const auto savePath = std::filesystem::current_path() / "saves" / "char_default.txt";
    REQUIRE(std::filesystem::exists(savePath));

    std::ifstream in(savePath);
    std::string line1;
    std::string line2;
    std::string line3;
    std::getline(in, line1);
    std::getline(in, line2);
    std::getline(in, line3);

    REQUIRE(line1 == "1 1 0");
    REQUIRE(line2 == "Warrior 120");
    REQUIRE(line3 == "Mage 80");
    REQUIRE(captured.str().find("Invalid choice, defaulting to Warrior.") != std::string::npos);
}

TEST_CASE("Game::startGame nieznany wybor menu przechodzi sciezke nowej gry", "[game]") {
    ScopedCurrentPath cwd;
    ScopedCin input("99\n1\n1\n1\n4\nmenu_default\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    const auto savePath = std::filesystem::current_path() / "saves" / "menu_default.txt";
    REQUIRE(std::filesystem::exists(savePath));
    REQUIRE(captured.str().find("Game saved. Exiting...") != std::string::npos);
}

TEST_CASE("Game::startGame load poprawnego save zachowuje naglowek po zapisie", "[game]") {
    ScopedCurrentPath cwd;
    writeSaveFile("seed_slot.txt", "7 1 0\nWarrior 90\nMage 70\n");
    ScopedCin input("2\n1\n4\nloaded_slot\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    const auto savePath = std::filesystem::current_path() / "saves" / "loaded_slot.txt";
    REQUIRE(std::filesystem::exists(savePath));

    std::ifstream in(savePath);
    std::string header;
    std::getline(in, header);
    REQUIRE(header == "7 1 0");
    REQUIRE(captured.str().find("Game saved. Exiting...") != std::string::npos);
}

TEST_CASE("Game::startGame load uszkodzonego save konczy gre", "[game]") {
    ScopedCurrentPath cwd;
    writeSaveFile("broken_slot.txt", "7 1\n");
    ScopedCin input("2\n1\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    REQUIRE(captured.str().find("Save file is corrupted") != std::string::npos);
    REQUIRE(captured.str().find("Load failed. Exiting game.") != std::string::npos);
}

TEST_CASE("Game::startGame load save z nieznanym typem postaci konczy gre", "[game]") {
    ScopedCurrentPath cwd;
    writeSaveFile("unknown_slot.txt", "3 1 0\nKnight 50\nMage 40\n");
    ScopedCin input("2\n1\n");
    ScopedCout captured;

    Game game;
    game.startGame();

    REQUIRE(captured.str().find("unknown character type") != std::string::npos);
    REQUIRE(captured.str().find("Load failed. Exiting game.") != std::string::npos);
}

