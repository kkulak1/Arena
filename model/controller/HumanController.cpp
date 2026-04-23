//
// Created by kamil on 20.03.2026.
//

#include "../../include/controller/HumanController.h"

#include <iostream>
#include <limits>

#include "../../include/ConsoleRenderer.h"

namespace {
    void printActionMenu(Character& character) {
        std::string specialOption = "3. Special Ability";
        if (!character.canUseSpecial()) {
            specialOption += " (cooldown: " + std::to_string(character.getSpecialCooldownRemaining()) + ")";
        }

        ConsoleRenderer::printMessage("\n" + character.getName() + " choose action:", Color::Default, &character);
        ConsoleRenderer::printMessage(
            "1. Attack\n2. Defend\n" + specialOption + "\n4. Save game\n",
            Color::Default,
            &character
        );
    }

    TurnDecision buildDecisionFromChoice(int choice, Character& character) {
        TurnDecision result;

        switch (choice) {
            case 1:
                result.action = EAction::ATTACK;
                break;
            case 2:
                result.action = EAction::DEFEND;
                break;
            case 3:
                if (character.canUseSpecial()) {
                    result.action = EAction::SPECIAL;
                } else {
                    ConsoleRenderer::printMessage(
                        "Special ability is on cooldown for " + std::to_string(character.getSpecialCooldownRemaining()) +
                        " more turn(s). Using Attack instead.",
                        Color::Default,
                        &character
                    );
                    result.action = EAction::ATTACK;
                }
                break;
            case 4:
                result.command = EGameCommand::SAVE_AND_EXIT;
                break;
            default:
                result.action = EAction::ATTACK;
                break;
        }

        return result;
    }
}

TurnDecision HumanController::decideTurn(Character &character, Character& enemy) {
    int choice = 1;
    (void) enemy;

    printActionMenu(character);
    while (true) {
        if (std::cin >> choice && choice >= 1 && choice <= 4) {
            break;
        }
        if (std::cin.eof()) {
            choice = 1;
            break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        ConsoleRenderer::printMessage("Invalid input. Please enter a number between 1 and 4.", Color::Default, &character);
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return buildDecisionFromChoice(choice, character);
}
