#include "raylib.h"

#include "camera.hpp"

EpicCamera::operator Camera2D() {
    const float w = GetScreenWidth();
    const float h = GetScreenHeight();

    const Vector2 offset{w / 2.0f, h / 2.0f};

    return Camera2D{
        .offset = offset,
        .target = center,
        .rotation = 0.0,
        .zoom = zoom,
    };
}

EpicCamera camera;
