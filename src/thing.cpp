#include <iostream>
#include <ostream>
#include <variant>
#include <vector>

#include "muli/settings.h"
#include "raylib.h"

#include "misc.hpp"
#include "thing.hpp"

muli::World* Things::phys_world = nullptr;

Things::List Things::all;

void Things::erase(std::size_t idx) {
    {
        const auto& old = all.at(idx);
        phys_world->Destroy(old.underlying_body);
    }

    all.erase(all.begin() + idx);
}

void Things::clear() {
    all.clear();
}

void Things::update() {
    phys_world->Step(TIMESTEP);
}

void Things::draw() {
    for (const auto& thing : all) {
        const auto body_pos = thing->GetPosition();
        const Vector2 pos{body_pos.x, body_pos.y};

        const float rot = thing->GetRotation().GetAngle() * RAD2DEG;

        const auto& draw = thing.draw;

        if (std::holds_alternative<RectDraw>(draw)) {
            const float w = thing.width;
            const float h = thing.height;
            DrawRectanglePro({pos.x, pos.y, w, h}, {w * 0.5f, h * 0.5f}, rot, BLACK);
        } else {
            const auto texture = std::get<Texture>(draw);
            DrawTextureEx(texture, pos, rot, 1.0, WHITE);
        }
    }
}
