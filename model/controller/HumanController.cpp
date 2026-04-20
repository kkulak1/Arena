//
// Created by kamil on 20.03.2026.
//

#include "../../include/controller/HumanController.h"

#include <iostream>

#include "../../include/ConsoleRenderer.h"

TurnDecision HumanController::decideTurn(Character &character, Character& enemy) {
    int choice;

    ConsoleRenderer::printMessage("\n" + character.getName() + " choose action:", Color::Default, &character);
    ConsoleRenderer::printMessage("1. Attack\n2. Defend\n3. Special Ability\n4. Save game\n", Color::Default, &character);
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
