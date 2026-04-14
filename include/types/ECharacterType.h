//
// Created by kamil on 02.04.2026.
//

#ifndef ARENA_ECHARACTERTYPE_H
#define ARENA_ECHARACTERTYPE_H

#include <string_view>

enum class ECharacterType {
    WARRIOR,
    MAGE,
    ARCHER
};

inline std::string_view toString(ECharacterType type) {
    switch (type) {
        case ECharacterType::WARRIOR: return "Warrior";
        case ECharacterType::MAGE: return "Mage";
        case ECharacterType::ARCHER: return "Archer";
    }
    return "Unknown";
}

#endif //ARENA_ECHARACTERTYPE_H