//
// Created by kamil on 16.03.2026.
//

#include "../include/Game.h"

#include <iostream>
#include <limits>

#include "ConsoleRenderer.h"
#include "../include/Arena.h"
#include "../include/SaveManager.h"
#include "../include/character/Archer.h"
#include "../include/character/Mage.h"
#include "../include/character/Warrior.h"
#include "../include/controller/EasyAIController.h"
#include "../include/controller/HardAIController.h"
#include "../include/controller/HumanController.h"
#include "windows.h"
#include <ctime>

namespace {
    void enableANSI() {     // aktywuje kolory w terminalu
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    bool readIntChoice(int& value) {
        if (std::cin >> value) {
            return true;
        }

        if (std::cin.eof()) {
            value = 3;
            return true;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }

    std::unique_ptr<Character> createCharacterFromChoice(int choice) {
        switch (choice) {
            case 1: return std::make_unique<Warrior>("Warrior");
            case 2: return std::make_unique<Mage>("Mage");
            case 3: return std::make_unique<Archer>("Archer");
            default: return std::make_unique<Warrior>("Warrior");
        }
    }
}

int Game::showMenu() {
    int choice = -1;
    ConsoleRenderer::printMessage("Welcome to the Arena!\n"
                                  "1. Start Game\n"
                                  "2. Load Game\n"
                                  "3. Exit\n", Color::Default);

    if (!readIntChoice(choice)) {
        ConsoleRenderer::printMessage("Invalid input. Please enter a number.\n", Color::Default);
        return -1;
    }

    return choice;
}

int Game::chooseMode() {
    ConsoleRenderer::printMessage("Choose game mode:\n"
                                  "1. Player vs Player\n"
                                  "2. Player vs AI\n", Color::Default);

    int choice = 2;
    if (!readIntChoice(choice)) {
        ConsoleRenderer::printMessage("Invalid choice, defaulting to Player vs AI.\n", Color::Default);
        return 2;
    }

    if (choice == 1) {
        return 1;
    } else if (choice == 2) {
        return 2;
    } else {
        ConsoleRenderer::printMessage("Invalid choice, defaulting to Player vs AI.\n", Color::Default);
        return 2;
    }
}

std::unique_ptr<Controller> Game::chooseAIDifficulty() {
    ConsoleRenderer::printMessage("Choose AI difficulty:\n"
                                  "1. Easy\n"
                                  "2. Hard\n", Color::Default);

    int choice = 1;
    if (!readIntChoice(choice)) {
        ConsoleRenderer::printMessage("Invalid choice, defaulting to Easy AI.\n", Color::Default);
        return std::make_unique<EasyAIController>();
    }

    if (choice == 1) {
        return std::make_unique<EasyAIController>();
    } else if (choice == 2) {
        return std::make_unique<HardAIController>();
    } else {
        ConsoleRenderer::printMessage("Invalid choice, defaulting to Easy AI.\n", Color::Default);
        return std::make_unique<EasyAIController>();
    }
}

std::unique_ptr<Character> Game::createCharacter(int playerNumber) {
    ConsoleRenderer::printMessage("Player " + std::to_string(playerNumber) + ", choose your character:\n"
                                  "1. Warrior\n"
                                  "2. Mage\n"
                                  "3. Archer\n", Color::Default);

    int choice = 1;
    if (!readIntChoice(choice)) {
        ConsoleRenderer::printMessage("Invalid choice, defaulting to Warrior.\n", Color::Default);
        std::unique_ptr<Character> fallback = std::make_unique<Warrior>("Warrior");
        fallback->setTeamId(playerNumber);
        return fallback;
    }

    if (choice < 1 || choice > 3) {
        ConsoleRenderer::printMessage("Invalid choice, defaulting to Warrior.\n", Color::Default);
        choice = 1;
    }

    std::unique_ptr<Character> character = createCharacterFromChoice(choice);

    if (character) character->setTeamId(playerNumber);
    return character;
}

bool Game::handleLoadGame(std::unique_ptr<Character>& player1, std::unique_ptr<Character>& player2, std::unique_ptr<Controller>& controller2, int& turn, int& mode, int& aiDifficulty) {
    GameState state = SaveManager::loadGame();
    if (state.p1 == nullptr || state.p2 == nullptr) {
        ConsoleRenderer::printMessage("Load failed. Returning to menu.\n", Color::Default);
        return false;
    }
    player1 = std::move(state.p1);    // kopiowanie jest zabronione przy uniqe_ptr -> move()
    player2 = std::move(state.p2);
    player1->setTeamId(1);
    player2->setTeamId(2);
    turn = state.turn;
    mode = state.mode;
    aiDifficulty = state.aiDifficulty;

    if (mode == 1) {
        controller2 = std::make_unique<HumanController>();
    } else {
        controller2 = (aiDifficulty == 2) ? std::unique_ptr<Controller>(std::make_unique<HardAIController>())
                                          : std::unique_ptr<Controller>(std::make_unique<EasyAIController>());
    }
    return true;
}

void Game::handleNewGame(std::unique_ptr<Character>& player1, std::unique_ptr<Character>& player2, std::unique_ptr<Controller>& controller2, int& mode, int& aiDifficulty) {
    mode = chooseMode();
    player1 = createCharacter(1);

    if (mode == 1) {
        player2 = createCharacter(2);
        aiDifficulty = 0;
        controller2 = std::make_unique<HumanController>();
    } else {    // AI losowy wybor postaci
        int randomClass = (std::rand() % 3) + 1;
        player2 = createCharacterFromChoice(randomClass);
        player2->setTeamId(2);
        ConsoleRenderer::printMessage("AI randomly chose " + player2->getName() + ".\n", Color::Default);

        std::unique_ptr<Controller> ai = chooseAIDifficulty();
        aiDifficulty = dynamic_cast<HardAIController*>(ai.get()) ? 2 : 1;
        controller2 = std::move(ai);    // kopiowanie jest zabronione przy uniqe_ptr
    }
}

void Game::runGameLoop(Character *player1, Character *player2, Controller *controller1, Controller *controller2,
    int &turn, int mode, int aiDifficulty) {
    Arena arena(player1, player2, controller1, controller2, turn, mode, aiDifficulty);

    while (true) {
        EGameCommand result = arena.step();

        if (result == EGameCommand::SAVE_AND_EXIT) {
            SaveManager::saveGame(
                player1,
                player2,
                turn,
                mode,
                aiDifficulty
            );

            ConsoleRenderer::printMessage("Game saved. Exiting...\n", Color::Default);
            break;
        }

        if (result == EGameCommand::GAME_OVER) {
            ConsoleRenderer::printMessage("Game over!\n", Color::Default);
            break;
        }
    }
}

void Game::startGame() {
    enableANSI();
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (true) {
        int menu = showMenu();
        if (menu == -1) {   // fallback
            continue;
        }
        if (menu == 3) {    // exit
            ConsoleRenderer::printMessage("Exiting game. Goodbye!\n", Color::Default);
            return;
        }

        std::unique_ptr<Character> player1 = nullptr;
        std::unique_ptr<Character> player2 = nullptr;
        int turn = 1;
        int mode = 2;
        int aiDifficulty = 1;
        std::unique_ptr<Controller> controller1 = std::make_unique<HumanController>();
        std::unique_ptr<Controller> controller2 = nullptr;

        bool success = false;
        if (menu == 2) {
            success = handleLoadGame(player1, player2, controller2, turn, mode, aiDifficulty);
        } else if (menu == 1) {
            handleNewGame(player1, player2, controller2, mode, aiDifficulty);
            success = true;
        } else {
            ConsoleRenderer::printMessage("Invalid menu choice. Please choose 1, 2 or 3.\n", Color::Default);
        }

        if (!success) { //fallback
            continue;
        }

        runGameLoop(player1.get(), player2.get(), controller1.get(), controller2.get(), turn,mode, aiDifficulty);
    }
}
