#pragma once

#include <memory>
#include <vector>

#include "raylib.h"

struct Exit {};

extern enum GameState {
    RESTART,
    PLAY,
} game_state;

void tick_things();

void reset();
void ready_set_go();

void start();

void follow_player();
void draw_lines();
void cull_lines();
