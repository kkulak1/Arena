//
// Created by kamil on 01.04.2026.
//

#include "../include/SaveManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include "../include/character/Warrior.h"
#include "../include/character/Mage.h"
#include "../include/character/Archer.h"

void SaveManager::saveGame(Character *p1, Character *p2, int turn) {
    std::string filename;

    std::cout << "Enter filename to save the game: ";
    std::cin >> filename;

    std::ofstream file("saves/" + filename + ".txt");

    if (!file) {
        std::cout << "Error while saving!\n";
        return;
    }

    file << toString(p1->getCharacterType()) << " " << p1->getHp() << "\n";
    file << toString(p2->getCharacterType()) << " " << p2->getHp() << "\n";
    file << turn << "\n";

    file.close();

    std::cout << "Game saved successfully!\n";
}

std::string SaveManager::chooseSave() {
    const std::vector<std::string> saves = listSaves();
    int choice;

    if (saves.empty()) {
        std::cout << "No saves found!\n";
        return "";
    }

    std::cout << "Available saves:\n";
    for (int i = 0; i < saves.size(); i++) {
        std::cout << i + 1 << "." << saves[i] << "\n";
    }

    std::cout << "Choose a save to load: ";
    std::cin >> choice;
    if (choice < 1 || choice > saves.size()) {
        std::cout << "Invalid choice!\n";
        return "";
    }

    return "saves/" + saves[choice - 1];
}

GameState SaveManager::loadGame(Character *&p1, Character *&p2, int &turn) {
    std::string path = chooseSave();
    std::ifstream file(path);
    std::string type1, type2;
    int hp1, hp2;

    file >> type1 >> hp1 >> type2 >> hp2 >> turn;

    if (type1 == "Warrior") {
        p1 = new Warrior(type1);
    } else if (type1 == "Mage") {
        p1 = new Mage(type1);
    } else if (type1 == "Archer") {
        p1 = new Archer(type1);
    }

    if (type2 == "Warrior") {
        p2 = new Warrior(type2);
    } else if (type2 == "Mage") {
        p2 = new Mage(type2);
    } else if (type2 == "Archer") {
        p2 = new Archer(type2);
    }

    p1->setHealth(hp1);
    p2->setHealth(hp2);

    GameState state;
    state.p1 = p1;
    state.p2 = p2;
    state.turn = turn;

    file.close();

    return state;
}

std::vector<std::string> SaveManager::listSaves() {
    std::vector<std::string> saves;

    if (!std::filesystem::exists("saves")) {
        return saves;
    }
    for (const auto& entry : std::filesystem::directory_iterator("saves")) {
        if (entry.is_regular_file()) {
            saves.push_back(entry.path().filename().string());
        }
    }
    return saves;
}
