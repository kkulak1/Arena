//
// Created by kamil on 14.04.2026.
//

#include "../include/ConsoleRenderer.h"

#include <iostream>

#include "../include/types/Color.h"

namespace {
    Color decideColor(const Character& player) {
        Color color = Color::Default;
        if (player.getTeamId() == 1) color = Color::Red;
        else if (player.getTeamId() == 2) color = Color::Blue;
        return color;
    }
}

void ConsoleRenderer::setColor(Color color) {
    std::cout << "\033[" << static_cast<int>(color) << "m";
}

void ConsoleRenderer::reset() {
    std::cout << "\033[0m";
}

void ConsoleRenderer::printPlayer(const Character &player) {
    Color color = decideColor(player);

    setColor(color);
    std::cout << player.getName() << " - HP: "
              << player.getHp() << "/" << player.getMaxHp()
              << "\n";
    reset();
}

void ConsoleRenderer::printMessage(const std::string &text, Color color) {
    setColor(color);
    std::cout << text << "\n";
    reset();
}

void ConsoleRenderer::printMessage(const std::string &text, Color color, const Character* player) {
    const Color finalColor = (player != nullptr) ? decideColor(*player) : color;
    printMessage(text, finalColor);
}
