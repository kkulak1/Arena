//
// Created by kamil on 14.04.2026.
//

#ifndef ARENA_CONSOLERENDERER_H
#define ARENA_CONSOLERENDERER_H
#include "character/Character.h"
#include "types/Color.h"


class ConsoleRenderer {
private:
    static void setColor(Color color);
    static void reset();
public:
    static void printPlayer(const Character& player);
    static void printMessage(const std::string& text, Color color);
    static void printMessage(const std::string& text, Color color, const Character* player);
};


#endif //ARENA_CONSOLERENDERER_H