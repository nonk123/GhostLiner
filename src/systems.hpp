#pragma once

#include <memory>
#include <vector>

#include "raylib.h"

#include "thing.hpp"

struct Exit {};

extern enum GameState {
    RESTART,
    PLAY,
} game_state;

void draw_things(Query<With<Body>>);

void reset(Commands, Query<All>);
void ready_set_go();

void start(Commands);
void follow_player(Query<With<Player, Body>>);

void draw_lines(Commands);
void cull_lines(Commands, Query<With<Line, Body>>);

void explode(Commands, Query<With<Explosive, Body>>, Query<With<Line, Body>>);

void await_restart(Query<With<Player>>);
