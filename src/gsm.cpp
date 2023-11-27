#include "raylib.h"

#include "gsm.hpp"
#include "thing.hpp"

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
    Thing player;

    auto& body = player.body;
    body.Set({1.0, 1.0}, 20.0);
    body.draw = RectDraw{};

    Things::spawn(player);
}

void Play::update() {
    // throw Exit();
}

GSM gsm;
