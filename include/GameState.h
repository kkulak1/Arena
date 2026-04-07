//
// Created by kamil on 01.04.2026.
//

#ifndef ARENA_GAMESTATE_H
#define ARENA_GAMESTATE_H
#include "character/Character.h"

struct GameState {
    Character* p1;
    Character* p2;
    int turn;
};

#endif //ARENA_GAMESTATE_H