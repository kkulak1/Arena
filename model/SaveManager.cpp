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

namespace {
    struct SaveData {
        int turn = 0;
        int mode = 2;
        int aiDifficulty = 1;
        std::string type1;
        std::string type2;
        int hp1 = 0;
        int hp2 = 0;
    };

    bool readSaveData(std::istream& in, SaveData& out) {
        in >> out.turn >> out.mode >> out.aiDifficulty;
        in >> out.type1 >> out.hp1;
        in >> out.type2 >> out.hp2;
        return static_cast<bool>(in);
    }

    std::filesystem::path getSaveDir() {
        std::filesystem::path saveDir = std::filesystem::current_path() / "saves";
        std::filesystem::create_directories(saveDir);
        return saveDir;
    }

    Character* createCharacterFromString(const std::string& type) {
        if (type == "Warrior") return new Warrior(type);
        if (type == "Mage") return new Mage(type);
        if (type == "Archer") return new Archer(type);
        return nullptr;
    }
}

void SaveManager::saveGame(Character *p1, Character *p2, int turn, int mode, int aiDifficulty, const std::string& filename) {
    const std::filesystem::path filePath = getSaveDir() / (filename + ".txt");
    std::ofstream file(filePath);
    if (!file) {
        std::cout << "Error while saving to: " << filePath << "\n";
        return;
    }

    file << turn << " " << mode << " " << aiDifficulty << "\n";
    file << toString(p1->getCharacterType()) << " " << p1->getHp() << "\n";
    file << toString(p2->getCharacterType()) << " " << p2->getHp() << "\n";

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

    return (getSaveDir() / saves[choice - 1]).string();
}

GameState SaveManager::loadGame() {
    std::string path = chooseSave();
    if (path.empty()) {
        return {};
    }

    std::ifstream file(path);
    if (!file) {
        std::cout << "Could not open save file: " << path << "\n";
        return {};
    }

    SaveData data;
    if (!readSaveData(file, data)) {
        std::cout << "Save file is corrupted: " << path << "\n";
        return {};
    }

    GameState state;
    state.turn = data.turn;
    state.mode = data.mode;
    state.aiDifficulty = data.aiDifficulty;
    state.p1 = createCharacterFromString(data.type1);
    state.p2 = createCharacterFromString(data.type2);

    if (state.p1 == nullptr || state.p2 == nullptr) {
        std::cout << "Save file contains unknown character type(s): " << data.type1 << ", " << data.type2 << "\n";
        delete state.p1;
        delete state.p2;
        return {};
    }

    state.p1->setHealth(data.hp1);
    state.p2->setHealth(data.hp2);
    file.close();
    return state;
}

std::vector<std::string> SaveManager::listSaves() {
    std::vector<std::string> saves;

    const std::filesystem::path saveDir = getSaveDir();
    for (const auto& entry : std::filesystem::directory_iterator(saveDir)) {
        if (entry.is_regular_file()) {
            saves.push_back(entry.path().filename().string());
        }
    }
    return saves;
}
