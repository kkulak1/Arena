//
// Created by kamil on 16.03.2026.
//

#include "../include/Game.h"

#include <iostream>

#include "../include/Arena.h"
#include "../include/character/Archer.h"
#include "../include/character/Mage.h"
#include "../include/character/Warrior.h"
#include "../include/controller/HumanController.h"

int Game::showMenu() {
    int choice;

    std::cout << "Welcome to the Arena!\n";
    std::cout << "1. Start Game\n";
    std::cout << "2. Exit\n";

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
    int menu = showMenu();

    if (menu != 1) {
        std::cout << "Exiting game. Goodbye!\n";
        return;
    }

    int mode = chooseMode();

    Character *player1 = createCharacter(1);
    Character *player2 = createCharacter(2);

    Controller* controller1 = new HumanController();
    Controller* controller2;

    if (mode == 1)
        controller2 = new HumanController();
    else
        controller2 = new AIController();

    Arena arena(player1, player2, controller1, controller2);

    startMatch(&arena);

    delete controller1;
    delete controller2;
    delete player1;
    delete player2;
}

void Game::startMatch(Arena *arena) {
    arena->startGame();
}
