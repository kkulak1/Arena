//
// Created by kamil on 16.03.2026.
//

#include "../include/Arena.h"

#include <fstream>
#include <iostream>

#include "../include/EAction.h"
#include "../include/SaveManager.h"
#include "../include/controller/EasyAIController.h"

Arena::Arena(Character *p1, Character *p2, Controller *c1, Controller *c2)
    : player1(p1), player2(p2), controller1(c1), controller2(c2), turn(1) {}

void Arena::printStatus() const {
    std::cout << "Status:\n";
    std::cout << player1->getName() << " - HP: " << player1->getHp() << "/" << player1->getMaxHp() << "\n";
    std::cout << player2->getName() << " - HP: " << player2->getHp() << "/" << player2->getMaxHp() << "\n";
}

void Arena::startGame() {
    while (player1->isAlive() && player2-> isAlive()) {
        std::cout << "\nTurn " << turn << ":\n";
        printStatus();
        std::cout << "\nPress 's' to save the game or any other key to continue: ";
        char choice;
        std::cin >> choice;
        if (choice == 's' || choice == 'S') {
            SaveManager::saveGame(player1, player2, turn);
            std::cout << "Game saved. Exiting...\n";
            return;
        }

        executeTurn(*player1, *player2, controller1);
        if (!player2->isAlive()) break;

        executeTurn(*player2, *player1, controller2);
        if (!player1->isAlive()) break;

        turn++;
    }

    std::cout << "\nGame Over! ";
    if (player1->isAlive()) {
        std::cout << player1->getName() << " wins!\n";
    } else {
        std::cout << player2->getName() << " wins!\n";
    }
}

void Arena::executeTurn(Character &attacker, Character &defender, Controller* controller) {
    EAction action = controller->chooseAction(attacker, defender);
    executeAction(action, attacker, defender);
}

void Arena::executeAction(EAction action, Character &attacker, Character &defender) {
    switch (action) {
        case EAction::ATTACK:
            attacker.attackTarget(defender);
            break;
        case EAction::DEFEND:
            attacker.defend();
            std::cout << attacker.getName() << " is defending this turn.\n";
            break;
        case EAction::SPECIAL:
            attacker.specialAbility(defender);
            break;
        default:
            std::cout << "Invalid choice, defaulting to Attack.\n";
            attacker.attackTarget(defender);
            break;
    }
}

