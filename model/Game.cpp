//
// Created by kamil on 16.03.2026.
//

#include "../include/Game.h"

#include <iostream>

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
}

int Game::showMenu() {
    int choice;

    std::cout << "Welcome to the Arena!\n";
    std::cout << "1. Start Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";

    std::cin >> choice;
    return choice;
}

int Game::chooseMode() {
    std::cout << "Choose game mode:\n";
    std::cout << "1. Player vs Player\n";
    std::cout << "2. Player vs AI\n";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        return 1;
    } else if (choice == 2) {
        return 2;
    } else {
        std::cout << "Invalid choice, defaulting to Player vs AI.\n";
        return 2;
    }
}

Controller* Game::chooseAIDifficulty() {
    std::cout << "Choose AI difficulty:\n";
    std::cout << "1. Easy\n";
    std::cout << "2. Hard\n";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        return new EasyAIController();
    } else if (choice == 2) {
        return new HardAIController();
    } else {
        std::cout << "Invalid choice, defaulting to Easy AI.\n";
        return new EasyAIController();
    }
}

Character *Game::createCharacter(int playerNumber) {
    std::cout << "Player " << playerNumber << ", choose your character:\n";
    std::cout << "1. Warrior\n";
    std::cout << "2. Mage\n";
    std::cout << "3. Archer\n";

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            return new Warrior("Warrior");
        case 2:
            return new Mage("Mage");
        case 3:
            return new Archer("Archer");
        default: {;
            std::cout << "Invalid choice, defaulting to Warrior.\n";
            return new Warrior("Warrior");
        }
    }
}

void Game::startGame() {
    enableANSI();
    int menu = showMenu();

    if (menu == 3) {
        std::cout << "Exiting game. Goodbye!\n";
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
        if (state.p1 == nullptr || state.p2 == nullptr) {   // if fails to load save exit game
            std::cout << "Load failed. Exiting game.\n";
            delete controller1;
            delete controller2;
            delete player1;
            delete player2;
            return;
        }
        player1 = state.p1;
        player2 = state.p2;
        turn = state.turn;
        mode = state.mode;
        aiDifficulty = state.aiDifficulty;

        if (mode == 1) {
            controller2 = new HumanController();
        } else {
            controller2 = (aiDifficulty == 2) ? static_cast<Controller*>(new HardAIController())
                                             : static_cast<Controller*>(new EasyAIController());
        }
    } else {
        // New game
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
    }

    Arena arena(player1, player2, controller1, controller2, turn, mode, aiDifficulty);
    startMatch(&arena);

    delete controller1;
    delete controller2;
    delete player1;
    delete player2;
}

void Game::startMatch(Arena *arena) {
    arena->startGame();
}
