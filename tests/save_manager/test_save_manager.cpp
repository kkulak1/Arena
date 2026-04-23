#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "SaveManager.h"
#include "character/Mage.h"
#include "character/Warrior.h"
#include "types/ECharacterType.h"

namespace {
class ScopedCurrentPath {
public:
    ScopedCurrentPath() {
        originalPath_ = std::filesystem::current_path();
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        tempPath_ = std::filesystem::temp_directory_path() / ("arena_save_manager_tests_" + std::to_string(stamp));
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
} // namespace


TEST_CASE("SaveManager::chooseSave zwraca pusty string gdy brak zapisow", "[save_manager]") {
    ScopedCurrentPath cwd;
    ScopedCin input("1\n");
    ScopedCout captured;

    const std::string selected = SaveManager::chooseSave();

    REQUIRE(selected.empty());
    REQUIRE(captured.str().find("No saves found!") != std::string::npos);
}

TEST_CASE("SaveManager::chooseSave zwraca sciezke przy poprawnym wyborze", "[save_manager]") {
    ScopedCurrentPath cwd;
    std::filesystem::create_directories("saves");
    std::ofstream("saves/only_slot.txt") << "dummy";

    ScopedCin input("1\n");
    const std::string selected = SaveManager::chooseSave();

    REQUIRE(selected == (std::filesystem::current_path() / "saves" / "only_slot.txt").string());
}

TEST_CASE("SaveManager::chooseSave odrzuca wybor spoza zakresu", "[save_manager]") {
    ScopedCurrentPath cwd;
    std::filesystem::create_directories("saves");
    std::ofstream("saves/only_slot.txt") << "dummy";

    SECTION("za male") {
        ScopedCin input("0\n");
        ScopedCout captured;

        const std::string selected = SaveManager::chooseSave();

        REQUIRE(selected.empty());
        REQUIRE(captured.str().find("Invalid choice!") != std::string::npos);
    }

    SECTION("za duze") {
        ScopedCin input("2\n");
        ScopedCout captured;

        const std::string selected = SaveManager::chooseSave();

        REQUIRE(selected.empty());
        REQUIRE(captured.str().find("Invalid choice!") != std::string::npos);
    }
}

TEST_CASE("SaveManager::chooseSave odrzuca nienumeryczny wybor", "[save_manager]") {
    ScopedCurrentPath cwd;
    std::filesystem::create_directories("saves");
    std::ofstream("saves/only_slot.txt") << "dummy";

    ScopedCin input("abc\n");
    ScopedCout captured;

    const std::string selected = SaveManager::chooseSave();

    REQUIRE(selected.empty());
    REQUIRE(captured.str().find("Invalid choice!") != std::string::npos);
}

TEST_CASE("SaveManager::chooseSave akceptuje wybor z bialymi znakami", "[save_manager]") {
    ScopedCurrentPath cwd;
    std::filesystem::create_directories("saves");
    std::ofstream("saves/only_slot.txt") << "dummy";

    ScopedCin input("   1   \n");
    const std::string selected = SaveManager::chooseSave();

    REQUIRE(selected == (std::filesystem::current_path() / "saves" / "only_slot.txt").string());
}

TEST_CASE("SaveManager::loadGame zwraca domyslny stan gdy nie wybrano pliku", "[save_manager]") {
    ScopedCurrentPath cwd;
    ScopedCin input("1\n");

    const GameState state = SaveManager::loadGame();

    REQUIRE(state.p1 == nullptr);
    REQUIRE(state.p2 == nullptr);
    REQUIRE(state.turn == 0);
    REQUIRE(state.mode == 2);
    REQUIRE(state.aiDifficulty == 1);
}

TEST_CASE("SaveManager::loadGame wczytuje poprawny zapis", "[save_manager]") {
    ScopedCurrentPath cwd;
    std::filesystem::create_directories("saves");

    {
        std::ofstream out("saves/valid_slot.txt");
        out << "7 2 1\n";
        out << "Warrior 88\n";
        out << "Archer 61\n";
    }

    ScopedCin input("1\n");
    const GameState state = SaveManager::loadGame();

    REQUIRE(state.turn == 7);
    REQUIRE(state.mode == 2);
    REQUIRE(state.aiDifficulty == 1);
    REQUIRE(state.p1 != nullptr);
    REQUIRE(state.p2 != nullptr);
    REQUIRE(toString(state.p1->getCharacterType()) == "Warrior");
    REQUIRE(toString(state.p2->getCharacterType()) == "Archer");
    REQUIRE(state.p1->getHp() == 88);
    REQUIRE(state.p2->getHp() == 61);
}

TEST_CASE("SaveManager::loadGame zwraca domyslny stan dla uszkodzonego pliku", "[save_manager]") {
    ScopedCurrentPath cwd;
    std::filesystem::create_directories("saves");

    {
        std::ofstream out("saves/corrupted_slot.txt");
        out << "7 2\n";
    }

    ScopedCin input("1\n");
    ScopedCout captured;
    const GameState state = SaveManager::loadGame();

    REQUIRE(state.p1 == nullptr);
    REQUIRE(state.p2 == nullptr);
    REQUIRE(state.turn == 0);
    REQUIRE(state.mode == 2);
    REQUIRE(state.aiDifficulty == 1);
    REQUIRE(captured.str().find("Save file is corrupted") != std::string::npos);
}

TEST_CASE("SaveManager::loadGame zwraca domyslny stan dla nieznanego typu postaci", "[save_manager]") {
    ScopedCurrentPath cwd;
    std::filesystem::create_directories("saves");

    {
        std::ofstream out("saves/unknown_type_slot.txt");
        out << "3 1 2\n";
        out << "Knight 40\n";
        out << "Mage 35\n";
    }

    ScopedCin input("1\n");
    ScopedCout captured;
    const GameState state = SaveManager::loadGame();

    REQUIRE(state.p1 == nullptr);
    REQUIRE(state.p2 == nullptr);
    REQUIRE(state.turn == 0);
    REQUIRE(state.mode == 2);
    REQUIRE(state.aiDifficulty == 1);
    REQUIRE(captured.str().find("unknown character type") != std::string::npos);
}

