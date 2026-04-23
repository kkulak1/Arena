//
// Created by kamil on 01.04.2026.
//

#ifndef ARENA_GAMESTATE_H
#define ARENA_GAMESTATE_H
#include "../character/Character.h"
#include <memory>

struct GameState {
    std::unique_ptr<Character> p1;
    std::unique_ptr<Character> p2;
    int turn;
    int mode;
    int aiDifficulty;

    GameState(std::unique_ptr<Character> p1 = nullptr, std::unique_ptr<Character> p2 = nullptr, int turn = 0, int mode = 2, int aiDifficulty = 1)
        : p1(std::move(p1)), p2(std::move(p2)), turn(turn), mode(mode), aiDifficulty(aiDifficulty) {}
};

#endif //ARENA_GAMESTATE_H