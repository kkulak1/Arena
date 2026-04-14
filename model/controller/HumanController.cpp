//
// Created by kamil on 20.03.2026.
//

#include "../../include/controller/HumanController.h"

#include <iostream>

TurnDecision HumanController::decideTurn(Character &character, Character& enemy) {
    int choice;

    std::cout << character.getName() << ", choose action:\n";
    std::cout << "1. Attack\n";
    std::cout << "2. Defend\n";
    std::cout << "3. Special Ability\n";
    std::cout << "4. Save game\n";
    std::cin >> choice;

    TurnDecision result;
    switch (choice) {
        case 1: result.action = EAction::ATTACK; break;
        case 2: result.action = EAction::DEFEND; break;
        case 3: result.action = EAction::SPECIAL; break;
        case 4: result.command = EGameCommand::SAVE_AND_EXIT; break;
        default: result.action = EAction::ATTACK; break;
    }

    return result;
}
