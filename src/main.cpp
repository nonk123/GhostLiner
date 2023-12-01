#include "SECS.hpp"
#include "assets.hpp"
#include "muli/muli.h"
#include "raylib.h"

#include "camera.hpp"
#include "misc.hpp"
#include "systems.hpp"
#include "thing.hpp"

int main(int, char**) {
    phys_world = new muli::World(muli::WorldSettings{
        .gravity = {0.0, 9.8}, // Y goes down everywhere
        .sleeping = false,     // things are too dynamic to sleep in this game
    });

    InitWindow(800, 600, "GhostLiner");
    InitAudioDevice();

    SetTargetFPS(FPS);

    load_assets();

    auto stop = false;

    while (!stop && !WindowShouldClose()) {
        BeginDrawing();

        try {
            SECS::tick(game_state);
        } catch (Exit) {
            stop = true;
        }

        phys_world->Step(TIMESTEP);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    delete phys_world;
    phys_world = nullptr;

    return 0;
}
