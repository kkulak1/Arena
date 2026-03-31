//
// Created by kamil on 20.03.2026.
//

#include "../../include/controller/HumanController.h"

#include <iostream>

Action HumanController::chooseAction(Character &character, Character& enemy) {
    int choice;

    std::cout << character.getName() << ", choose action:\n";
    std::cout << "1. Attack\n";
    std::cout << "2. Defend\n";
    std::cout << "3. Special Ability\n";
    std::cin >> choice;

    switch (choice) {
        case 1: return Action::ATTACK;
        case 2: return Action::DEFEND;
        case 3: return Action::SPECIAL;
        default: return Action::ATTACK;
    }
}
