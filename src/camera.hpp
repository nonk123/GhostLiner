#pragma once

#include "raylib.h"

extern struct EpicCamera {
    float zoom{1.0};
    Vector2 center{0.0, 0.0};

    operator Camera2D();
} camera;
