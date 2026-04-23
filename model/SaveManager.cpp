//
// Created by kamil on 01.04.2026.
//

#include "../include/SaveManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include "../include/ConsoleRenderer.h"
#include "../include/character/Warrior.h"
#include "../include/character/Mage.h"
#include "../include/character/Archer.h"
#include "../include/types/Color.h"

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

    std::unique_ptr<Character> createCharacterFromString(const std::string& type) {
        if (type == "Warrior") return std::make_unique<Warrior>(type);
        if (type == "Mage") return std::make_unique<Mage>(type);
        if (type == "Archer") return std::make_unique<Archer>(type);
        return nullptr;
    }
}

void SaveManager::saveGame(Character *p1, Character *p2, int turn, int mode, int aiDifficulty) {
    std::string filename;
    ConsoleRenderer::printMessage("Enter filename to save the game:", Color::Default);
    std::cin >> filename;

    const std::filesystem::path filePath = getSaveDir() / (filename + ".txt");
    std::ofstream file(filePath);
    if (!file) {
        ConsoleRenderer::printMessage("Error while saving to: " + filePath.string(), Color::Default);
        return;
    }

    file << turn << " " << mode << " " << aiDifficulty << "\n";
    file << toString(p1->getCharacterType()) << " " << p1->getHp() << "\n";
    file << toString(p2->getCharacterType()) << " " << p2->getHp() << "\n";

    file.close();

    ConsoleRenderer::printMessage("Game saved successfully!", Color::Default);
}

std::string SaveManager::chooseSave() {
    const std::vector<std::string> saves = listSaves();
    int choice = -1;

    if (saves.empty()) {
        ConsoleRenderer::printMessage("No saves found!", Color::Default);
        return "";
    }

    ConsoleRenderer::printMessage("Available saves:", Color::Default);
    for (int i = 0; i < saves.size(); i++) {
        ConsoleRenderer::printMessage(std::to_string(i + 1) + "." + saves[i], Color::Default);
    }

    ConsoleRenderer::printMessage("\nChoose a save to load:", Color::Default);
    bool valid = static_cast<bool>(std::cin >> choice);
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!valid || choice < 1 || choice > saves.size()) {
        ConsoleRenderer::printMessage("Invalid choice!", Color::Default);
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
        ConsoleRenderer::printMessage("Could not open save file: " + path, Color::Default);
        return {};
    }

    SaveData data;
    if (!readSaveData(file, data)) {
        ConsoleRenderer::printMessage("Save file is corrupted: " + path, Color::Default);
        return {};
    }

    GameState state;
    state.turn = data.turn;
    state.mode = data.mode;
    state.aiDifficulty = data.aiDifficulty;
    state.p1 = createCharacterFromString(data.type1);
    state.p2 = createCharacterFromString(data.type2);

    if (state.p1 == nullptr || state.p2 == nullptr) {
        ConsoleRenderer::printMessage("Save file contains unknown character type(s): " + data.type1 + ", " + data.type2,
                                      Color::Default);
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
