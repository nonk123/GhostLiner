#include "gsm.hpp"
#include "camera.hpp"
#include "thing.hpp"

#include "raylib.h"
#include <cmath>

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
        Things::draw();

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
    if (GetTime() - init_time >= 3.0) {
        transition.replace(new Play);
    }
}

void Play::init() {
    const auto player = new Thing;

    auto& body = player->body;
    body.Set({1.0, 1.0}, 20.0);

    Things::spawn(player);

    last_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);
}

void Play::update() {
    const auto world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        const Vector2 delta{world_pos.x - last_world_pos.x, world_pos.y - last_world_pos.y};
        const float eps = 0.001;

        if (std::abs(delta.x) <= eps || std::abs(delta.y) <= eps) {
            return;
        }

        auto line = new Thing;
        auto& body = line->body;

        const Vec2 size(std::hypot(delta.x, delta.y), 0.5);

        body.Set(size, FLT_MAX);
        body.friction = 0.0;
        body.position = Vec2(world_pos.x, world_pos.y);
        body.rotation = std::atan2(delta.y, delta.x);

        Things::spawn(line);
    }

    last_world_pos = world_pos;
}

GSM gsm;
