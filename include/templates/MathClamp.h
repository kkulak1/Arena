//
// Created by kamil on 20.04.2026.
//

#ifndef ARENA_MATHCLAMP_H
#define ARENA_MATHCLAMP_H

template <typename T>
T clampMin(T value, T minValue) {
    return value < minValue ? minValue : value;
}

#endif //ARENA_MATHCLAMP_H

