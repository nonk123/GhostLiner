#include "box2d-lite/World.h"
#include "raylib.h"

#include "camera.hpp"
#include "gsm.hpp"
#include "misc.hpp"

int main(int, char**) {
    InitWindow(800, 600, "GhostLiner");
    InitAudioDevice();

    SetTargetFPS(FPS);

    auto stop = false;

    while (!WindowShouldClose() && !stop) {
        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(RAYWHITE);

        try {
            gsm.tick();
        } catch (Exit) {
            stop = true;
        }

        EndMode2D();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
