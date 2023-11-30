#include <algorithm>
#include <cmath>
#include <memory>

#include "camera.hpp"
#include "gsm.hpp"
#include "raymath.h"
#include "thing.hpp"

#include "muli/muli.h"
#include "raylib.h"

GameState game_state = GameState::RESTART;

void tick_things() {
    BeginMode2D(camera);

    Things::update();

    {
        ClearBackground(RAYWHITE);
        Things::draw();
    }

    EndMode2D();
}

static double init_time;

void reset() {
    Things::clear();
    init_time = GetTime();
}

void ready_set_go() {
    const auto secs = GetTime() - init_time;
    std::string text;

    if (secs <= 1.0) {
        text = "Get ready";
    } else if (secs <= 2.0) {
        text = "Set";
    } else if (secs <= 3.0) {
        text = "GOGOGO!!!";
    } else {
        game_state = GameState::PLAY;
        return;
    }

    const int font_size = 30;
    const auto text_w = MeasureText(text.c_str(), font_size);
    const auto middle = GetScreenWidth() / 2 - text_w / 2;
    DrawText(text.c_str(), middle, 10, font_size, BLACK);
}

static Vector2 last_world_pos;

void start() {
    auto sled = Things::spawn_dynamic(1.4, 0.4).lock();
    sled->tags.insert(Tag::PLAYER);
    sled->body->SetFriction(0.2);

    last_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);
}

void follow_player() {
    for (const auto& thing : Things::all) {
        if (thing->tags.contains(Tag::PLAYER)) {
            const auto pos = thing->body->GetPosition();
            camera.center = {pos.x, pos.y};
            return;
        }
    }
}

void cull_lines() {
    for (const auto& thing : Things::all) {
        if (thing->tags.contains(Tag::LINE)) {
            const auto lifetime = GetTime() - thing->creation_time;

            if (lifetime > 5.0) {
                thing->tags.insert(Tag::DELETE_ME);
            }
        }
    }
}

void draw_lines() {
    const auto world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        const auto delta = Vector2Subtract(world_pos, last_world_pos);
        const auto length = Vector2Length(delta);

        if (length <= 0.2) {
            return;
        }

        const Vector2 mid =
            Vector2Scale(Vector2Add(world_pos, last_world_pos), 0.5);

        auto line = Things::spawn_static(length, 0.5).lock();
        line->tags.insert(Tag::LINE);
        line->body->SetFriction(0.1);
        line->body->SetPosition({mid.x, mid.y});
        line->body->SetRotation(std::atan2(delta.y, delta.x));
    }

    last_world_pos = world_pos;
}
