//
// Created by kamil on 16.03.2026.
//

#include "../include/Arena.h"

#include <fstream>
#include <iostream>
#include <string>

#include "../include/types/EAction.h"
#include "../include/SaveManager.h"
#include "../include/controller/EasyAIController.h"
#include "../include/types/Color.h"

Arena::Arena(Character *p1, Character *p2, Controller *c1, Controller *c2, int startTurn, int mode, int aiDifficulty)
    : player1(p1), player2(p2), controller1(c1), controller2(c2),
      turn(startTurn < 1 ? 1 : startTurn), mode(mode), aiDifficulty(aiDifficulty) {}

void Arena::printStatus() const {
    ConsoleRenderer::printMessage(std::string("\nTurn ") + std::to_string(turn) + ":\nStatus: ", Color::Default);
    ConsoleRenderer::printPlayer(*player1);
    ConsoleRenderer::printPlayer(*player2);
}

void Arena::startGame() {
    while (player1->isAlive() && player2-> isAlive()) {
        printStatus();

        executeTurn(*player1, *player2, controller1);
        if (shouldExit) return;
        if (!player2->isAlive()) break;

        executeTurn(*player2, *player1, controller2);
        if (shouldExit) return;
        if (!player1->isAlive()) break;

        turn++;
    }

    ConsoleRenderer::printMessage("\nGame Over! ", Color::Default);
    if (player1->isAlive()) {
        ConsoleRenderer::printMessage(player1->getName() + " wins!", Color::Default, player1);
    } else {
        ConsoleRenderer::printMessage(player2->getName() + " wins!", Color::Default, player2);
    }
}

void Arena::executeTurn(Character &attacker, Character &defender, Controller* controller) {
    TurnDecision decision = controller->decideTurn(attacker, defender);
    if (decision.command == EGameCommand::SAVE_AND_EXIT) {
        saveCurrentGame();
        ConsoleRenderer::printMessage("Game saved. Exiting...", Color::Default);
        shouldExit = true;
        return;
    }

    executeAction(decision.action, attacker, defender);
}

void Arena::executeAction(EAction action, Character &attacker, Character &defender) {
    switch (action) {
        case EAction::ATTACK:
            attacker.attackTarget(defender);
            break;
        case EAction::DEFEND:
            attacker.defend();
            ConsoleRenderer::printMessage(attacker.getName() + " is defending this turn.", Color::Default, &attacker);
            break;
        case EAction::SPECIAL:
            attacker.specialAbility(defender);
            break;
        default:
            ConsoleRenderer::printMessage("Unknown action, defaulting to ATTACK.", Color::Default, &attacker);
            attacker.attackTarget(defender);
            break;
    }
}

void Arena::saveCurrentGame() {
    std::string filename;
    ConsoleRenderer::printMessage("Enter filename to save the game:", Color::Default);
    std::cin >> filename;

    SaveManager::saveGame(player1, player2, turn, mode, aiDifficulty, filename);
}
