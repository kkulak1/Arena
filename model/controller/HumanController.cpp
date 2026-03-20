//
// Created by kamil on 20.03.2026.
//

#include "../../include/controller/HumanController.h"

#include <iostream>

Action HumanController::chooseAction(Character &character) {
    int choice;

    std::cout << character.getName() << ", choose action:\n";
    std::cout << "1. Attack\n";
    std::cout << "2. Defend\n";
    std::cout << "3. Special Ability\n";
    std::cin >> choice;

    if (choice == 2) return Action::DEFEND;
    if (choice == 3) return Action::SPECIAL;
    return Action::ATTACK;
}
