#include "SECS.hpp"
#include "muli/muli.h"
#include "raylib.h"

#include "camera.hpp"
#include "gsm.hpp"
#include "misc.hpp"
#include "thing.hpp"

const Systems SECS::systems{
    {GameState::RESTART, Stage::STARTUP, reset},
    {GameState::PLAY, Stage::STARTUP, start},
    {GameState::RESTART, Stage::UPDATE, draw_things},
    {GameState::PLAY, Stage::UPDATE, draw_things},
    {GameState::PLAY, Stage::UPDATE, follow_player},
    {GameState::PLAY, Stage::UPDATE, cull_lines},
    {GameState::PLAY, Stage::UPDATE, draw_lines},
    {GameState::RESTART, Stage::UPDATE, ready_set_go},
};

int main(int, char**) {
    phys_world = new muli::World(muli::WorldSettings{
        .gravity = {0.0, 9.8}, // Y goes down everywhere
        .sleeping = false,
    });

    InitWindow(800, 600, "GhostLiner");
    InitAudioDevice();

    SetTargetFPS(FPS);

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
