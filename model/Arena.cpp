//
// Created by kamil on 16.03.2026.
//

#include "../include/Arena.h"

#include <fstream>
#include <iostream>
#include <string>

#include "ConsoleRenderer.h"
#include "../include/types/EAction.h"
#include "../include/SaveManager.h"
#include "../include/controller/EasyAIController.h"
#include "../include/templates/DamageCalculator.h"
#include "../include/types/Color.h"
#include "templates/MathClamp.h"

namespace {
    std::string getSpecialName(const Character& character) {
        switch (character.getCharacterType()) {
            case ECharacterType::WARRIOR:
                return "POWER STRIKE";
            case ECharacterType::MAGE:
                return "FIREBALL";
            case ECharacterType::ARCHER:
                return "DOUBLE SHOT";
            default:
                return "SPECIAL";
        }
    }

    bool tryDodgeAttack(Character& defender, bool wasDefending) {
        const double dodgeRoll = static_cast<double>(rand()) / RAND_MAX;
        if (dodgeRoll >= defender.getDodgeChance()) {
            return false;
        }

        if (wasDefending) {
            defender.setDefend(false);
        }

        ConsoleRenderer::printMessage(defender.getName() + " dodged the attack!", Color::Default, &defender);
        return true;
    }
}

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
    attacker.setDefend(false);
    attacker.tickSpecialCooldown();

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
            applyAttack(attacker, defender);
            break;
        case EAction::DEFEND:
            applyDefending(attacker);
            ConsoleRenderer::printMessage(attacker.getName() + " is defending this turn.", Color::Default, &attacker);
            break;
        case EAction::SPECIAL:
            applySpecialAbility(attacker, defender);
            break;
        default:
            ConsoleRenderer::printMessage("Unknown action, defaulting to ATTACK.", Color::Default, &attacker);
            applyAttack(attacker, defender);
            break;
    }
}

void Arena::applyAttack(Character &attacker, Character &defender) {
    const bool wasDefending = defender.isDefending();

    if (tryDodgeAttack(defender, wasDefending)) {
        return;
    }

    const int damage = calculatedDamage(attacker, defender,
        [](int base, const Character& currentAttacker, const Character&) {
            if (double critRoll = static_cast<double>(rand()) / RAND_MAX; critRoll < currentAttacker.getCritChance()) {
                ConsoleRenderer::printMessage("Critical hit", Color::Default);
                return base * 2;
            }
            return base;
        }
    );

    const int dealtDamage = applyDamage(defender, damage);

    ConsoleRenderer::printMessage(attacker.getName() + " deals " + std::to_string(dealtDamage) + " damage to " + defender.getName(), Color::Default, &attacker);
}

void Arena::applyDefending(Character &defender) {
    defender.defend();
}

void Arena::applySpecialAbility(Character &attacker, Character &defender) {
    if (!attacker.canUseSpecial()) {
        ConsoleRenderer::printMessage(attacker.getName() + " cannot use special for " + std::to_string(attacker.getSpecialCooldownRemaining()) + " more turn(s).", Color::Default, &attacker);
        applyAttack(attacker, defender);
        return;
    }

    const std::string specialName = getSpecialName(attacker);
    ConsoleRenderer::printMessage(attacker.getName() + " uses " + specialName + "!", Color::Default, &attacker);

    const int rawSpecialDamage = attacker.specialAbility(defender);
    const int specialDamage = calculatedDamageFromBase(rawSpecialDamage, attacker, defender,
        [](int base, const Character&, const Character& currentDefender) {
            return base - currentDefender.getDefense();
        }
    );
    const int dealtDamage = applyDamage(defender, specialDamage);

    attacker.startSpecialCooldown();
    ConsoleRenderer::printMessage(attacker.getName() + " deals " + std::to_string(dealtDamage) + " damage to " + defender.getName(), Color::Default, &attacker);
}

int Arena::applyDamage(Character &defender, int damage) {
    int finalDamage = clampMin(damage, 0);

    if (defender.isDefending()) {
        finalDamage /= 2;
        defender.setDefend(false);
    }

    const int hpBefore = defender.getHp();
    defender.takeDamage(finalDamage);
    return hpBefore - defender.getHp();
}


void Arena::saveCurrentGame() const {
    std::string filename;
    ConsoleRenderer::printMessage("Enter filename to save the game:", Color::Default);
    std::cin >> filename;

    SaveManager::saveGame(player1, player2, turn, mode, aiDifficulty, filename);
}
