#include <iostream>
#include <ostream>
#include <variant>
#include <vector>

#include "raylib.h"

#include "misc.hpp"
#include "thing.hpp"

muli::World* Things::phys_world = nullptr;

Things::List Things::all;

void Things::erase(std::size_t idx) {
    all.erase(all.begin() + idx);
}

void Things::clear() {
    all.clear();
}

void Things::update() {
    phys_world->Step(TIMESTEP);

    std::erase_if(all, [](const auto& t) {
        return t->tags.contains(Tag::DELETE_ME);
    });
}

void Things::draw() {
    for (const auto& thing : all) {
        const auto body_pos = thing->body->GetPosition();
        const Vector2 pos{body_pos.x, body_pos.y};

        const float rot = thing->body->GetRotation().GetAngle() * RAD2DEG;

        const auto& draw = thing->draw;

        if (std::holds_alternative<RectDraw>(draw)) {
            const float w = thing->width;
            const float h = thing->height;
            DrawRectanglePro(
                {pos.x, pos.y, w, h}, {w * 0.5f, h * 0.5f}, rot, BLACK
            );
        } else {
            const auto texture = std::get<Texture>(draw);
            DrawTextureEx(texture, pos, rot, 1.0, WHITE);
        }
    }
}
