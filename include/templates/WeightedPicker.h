//
// Created by kamil on 21.04.2026.
//

#ifndef ARENA_WEIGHTEDPICKER_H
#define ARENA_WEIGHTEDPICKER_H
#include <vector>

template<typename T>
T pickWeightedRandom(const std::vector<std::pair<T, int>>& options, const T& fallback) {
    int totalWeight = 0;

    for (const auto& [option, weight] : options) {
        totalWeight += weight;
    }

    if (options.empty() || totalWeight <= 0) {
        return fallback;
    }

    int roll = rand() % totalWeight;

    for (const auto& [option, weight] : options) {
        if (weight <= 0) {
            continue;
        }
        if (roll < weight) {
            return option;
        }
        roll -= weight;
    }
    return fallback;
}

#endif //ARENA_WEIGHTEDPICKER_H