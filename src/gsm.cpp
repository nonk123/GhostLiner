#include <algorithm>
#include <cmath>
#include <memory>

#include "camera.hpp"
#include "gsm.hpp"
#include "raymath.h"
#include "thing.hpp"

#include "muli/muli.h"
#include "raylib.h"

GSM::GSM() {
    stack.push_back(std::unique_ptr<GameState>(new Restart));
}

void GSM::tick() {
    if (stack.empty()) {
        throw Exit();
    }

    auto& state = stack.back();
    state->update();

    const auto push = state->transition._push;
    const auto replace = state->transition._replacement;

    if (state->transition._pop) {
        stack.pop_back();

        if (stack.empty()) {
            throw Exit();
        }
    } else if (push != nullptr) {
        stack.push_back(std::unique_ptr<GameState>());
    } else if (replace != nullptr) {
        state.reset(replace);
    } else {
        Things::update();

        BeginMode2D(camera);
        {
            ClearBackground(RAYWHITE);
            Things::draw();
        }
        EndMode2D();

        state->overlay();

        return;
    }

    stack.back()->init();
}

void Restart::init() {
    Things::clear();
    init_time = GetTime();
}

void Restart::update() {
    const auto secs = GetTime() - init_time;

    if (secs >= 3.0) {
        transition.replace(new Play);
    }
}

void Restart::overlay() {
    const auto secs = GetTime() - init_time;
    std::string text;

    if (secs <= 1.0) {
        text = "Get ready";
    } else if (secs <= 2.0) {
        text = "Set";
    } else if (secs <= 3.0) {
        text = "GOGOGO!!!";
    }

    const int font_size = 30;
    const auto text_w = MeasureText(text.c_str(), font_size);
    const auto middle = GetScreenWidth() / 2 - text_w / 2;
    DrawText(text.c_str(), middle, 10, font_size, BLACK);
}

void Play::init() {
    auto sled = Things::spawn_dynamic(1.4, 0.4).lock();
    sled->tags.insert(Tag::PLAYER);
    sled->body->SetFriction(0.2);

    last_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);
}

void Play::update() {
    for (const auto& thing : Things::all) {
        if (thing->tags.contains(Tag::PLAYER)) {
            const auto pos = thing->body->GetPosition();
            camera.center = {pos.x, pos.y};
        }

        if (thing->tags.contains(Tag::LINE)) {
            const auto lifetime = GetTime() - thing->creation_time;

            if (lifetime > 5.0) {
                thing->tags.insert(Tag::DELETE_ME);
            }
        }
    }

    const auto world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        const auto delta = Vector2Subtract(world_pos, last_world_pos);
        const auto length = Vector2Length(delta);

        if (length <= 0.2) {
            return;
        }

        const Vector2 mid = Vector2Scale(Vector2Add(world_pos, last_world_pos), 0.5);

        auto line = Things::spawn_static(length, 0.5).lock();
        line->tags.insert(Tag::LINE);
        line->body->SetFriction(0.1);
        line->body->SetPosition({mid.x, mid.y});
        line->body->SetRotation(std::atan2(delta.y, delta.x));
    }

    last_world_pos = world_pos;
}

GSM gsm;
