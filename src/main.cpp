#include "muli/muli.h"
#include "raylib.h"

#include "gsm.hpp"
#include "misc.hpp"
#include "thing.hpp"

int main(int, char**) {
    muli::World world(muli::WorldSettings{
        .gravity = {0.0, 9.8}, // Y goes down everywhere
        .sleeping = false,
    });

    Things::phys_world = &world;

    InitWindow(800, 600, "GhostLiner");
    InitAudioDevice();

    SetTargetFPS(FPS);

    auto stop = false;

    while (!WindowShouldClose() && !stop) {
        BeginDrawing();

        try {
            gsm.tick();
        } catch (Exit) {
            stop = true;
        }

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    Things::phys_world = nullptr;

    return 0;
}
