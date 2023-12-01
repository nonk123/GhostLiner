#pragma once

#include <vector>

#include "raylib.h"

void load_assets();

template <Sound&... Sounds>
struct Picker {
    operator const Sound&() {
        std::vector<Sound*> sounds{&Sounds...};
        const auto choice = GetRandomValue(0, sounds.size() - 1);
        return *sounds[choice];
    }
};

extern Sound explode1, explode2, explode3;

inline Picker<explode1, explode2, explode3> pick_explode;
