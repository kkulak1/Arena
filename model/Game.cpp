//
// Created by kamil on 16.03.2026.
//

#include "../include/Game.h"

#include <iostream>
#include <limits>

#include "../include/Arena.h"
#include "../include/SaveManager.h"
#include "../include/character/Archer.h"
#include "../include/character/Mage.h"
#include "../include/character/Warrior.h"
#include "../include/controller/EasyAIController.h"
#include "../include/controller/HardAIController.h"
#include "../include/controller/HumanController.h"
#include "windows.h"

namespace {
    void enableANSI() {     // enables colors
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

Controller* Game::chooseAIDifficulty() {
    ConsoleRenderer::printMessage("Choose AI difficulty:\n"
                                  "1. Easy\n"
                                  "2. Hard\n", Color::Default);

    int choice = 1;
    if (!readIntChoice(choice)) {
        ConsoleRenderer::printMessage("Invalid choice, defaulting to Easy AI.\n", Color::Default);
        return new EasyAIController();
    }

    if (choice == 1) {
        return new EasyAIController();
    } else if (choice == 2) {
        return new HardAIController();
    } else {
        ConsoleRenderer::printMessage("Invalid choice, defaulting to Easy AI.\n", Color::Default);
        return new EasyAIController();
    }
}

Character *Game::createCharacter(int playerNumber) {
    ConsoleRenderer::printMessage("Player " + std::to_string(playerNumber) + ", choose your character:\n"
                                  "1. Warrior\n"
                                  "2. Mage\n"
                                  "3. Archer\n", Color::Default);

    int choice = 1;
    if (!readIntChoice(choice)) {
        ConsoleRenderer::printMessage("Invalid choice, defaulting to Warrior.\n", Color::Default);
        Character* fallback = new Warrior("Warrior");
        fallback->setTeamId(playerNumber);
        return fallback;
    }

    Character* character = nullptr;
    switch (choice) {
        case 1:
            character = new Warrior("Warrior");
            break;
        case 2:
            character = new Mage("Mage");
            break;
        case 3:
            character = new Archer("Archer");
            break;
        default:
            ConsoleRenderer::printMessage("Invalid choice, defaulting to Warrior.\n", Color::Default);
            character = new Warrior("Warrior");
            break;
    }

    if (character) character->setTeamId(playerNumber);
    return character;
}

void Game::startGame() {
    enableANSI();
    while (true) {
        int menu = showMenu();

        if (menu == -1) {
            continue;
        }

        if (menu == 3) {
            ConsoleRenderer::printMessage("Exiting game. Goodbye!\n", Color::Default);
            return;
        }

        Character *player1 = nullptr;
        Character *player2 = nullptr;
        int turn = 1;
        int mode = 2;
        int aiDifficulty = 1;

        Controller* controller1 = new HumanController();
        Controller* controller2 = nullptr;

        if (menu == 2) {
            GameState state = SaveManager::loadGame();
            if (state.p1 == nullptr || state.p2 == nullptr) {
                ConsoleRenderer::printMessage("Load failed. Returning to menu.\n", Color::Default);
                delete controller1;
                delete controller2;
                delete player1;
                delete player2;
                continue;
            }
            player1 = state.p1;
            player2 = state.p2;
            player1->setTeamId(1);
            player2->setTeamId(2);
            turn = state.turn;
            mode = state.mode;
            aiDifficulty = state.aiDifficulty;

            if (mode == 1) {
                controller2 = new HumanController();
            } else {
                controller2 = (aiDifficulty == 2) ? static_cast<Controller*>(new HardAIController())
                                                 : static_cast<Controller*>(new EasyAIController());
            }
        } else if (menu == 1) {
            mode = chooseMode();
            player1 = createCharacter(1);
            player2 = createCharacter(2);

            if (mode == 1) {
                aiDifficulty = 0;
                controller2 = new HumanController();
            } else {
                Controller* ai = chooseAIDifficulty();
                aiDifficulty = dynamic_cast<HardAIController*>(ai) ? 2 : 1;
                controller2 = ai;
            }
        } else {
            ConsoleRenderer::printMessage("Invalid menu choice. Please choose 1, 2 or 3.\n", Color::Default);
            delete controller1;
            delete controller2;
            delete player1;
            delete player2;
            continue;
        }

        Arena arena(player1, player2, controller1, controller2, turn, mode, aiDifficulty);
        startMatch(&arena);

        delete controller1;
        delete controller2;
        delete player1;
        delete player2;
        return;
    }
}

void Game::startMatch(Arena *arena) {
    arena->startGame();
}
