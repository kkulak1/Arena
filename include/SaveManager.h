//
// Created by kamil on 01.04.2026.
//

#ifndef ARENA_SAVEMANAGER_H
#define ARENA_SAVEMANAGER_H
#include <vector>

#include "GameState.h"
#include "character/Character.h"


class SaveManager {
public:
    static void saveGame(Character* p1, Character* p2, int turn);
    static std::string chooseSave();
    static GameState loadGame(Character*& p1, Character*& p2, int& turn);
private:
    static std::vector<std::string> listSaves();
};


#endif //ARENA_SAVEMANAGER_H