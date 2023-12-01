#include "assets.hpp"

#include "raylib.h"

Sound explode1, explode2, explode3;

#define LOAD(var) var = LoadSound("assets/" #var ".wav")

void load_sounds() {
    LOAD(explode1);
    LOAD(explode2);
    LOAD(explode3);
}

#undef LOAD

void load_assets() {
    load_sounds();
}
