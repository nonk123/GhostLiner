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
    {GameState::RESTART, Stage::UPDATE, tick_things},
    {GameState::PLAY, Stage::UPDATE, tick_things},
    {GameState::PLAY, Stage::UPDATE, follow_player},
    {GameState::PLAY, Stage::UPDATE, cull_lines},
    {GameState::PLAY, Stage::UPDATE, draw_lines},
    {GameState::RESTART, Stage::UPDATE, ready_set_go},
};

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

    while (!stop && !WindowShouldClose()) {
        BeginDrawing();

        try {
            SECS::tick(game_state);
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
