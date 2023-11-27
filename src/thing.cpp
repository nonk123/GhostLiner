#include <variant>
#include <vector>

#include "misc.hpp"
#include "raylib.h"
#include "thing.hpp"

World Things::phys_world({0.0, -9.8}, 10);

Things::List Things::all;

void Things::spawn(Thing thing) {
    all.push_back(thing);

    auto& last = all.back();
    phys_world.Add(dynamic_cast<Body*>(&last.body));
}

void Things::erase(std::size_t idx) {
    {
        const auto& old = all.at(idx);

        // TODO: test if this actually works.
        std::erase(phys_world.bodies, &old.body);
    }

    all.erase(all.begin() + idx);
}

void Things::clear() {
    while (!all.empty()) {
        erase(all.size() - 1);
    }
}

void Things::update() {
    phys_world.Step(TIMESTEP);
}

void Things::draw() {
    for (const auto& thing : all) {
        const auto& body = thing.body;
        const auto& draw = body.draw;

        const float px_per_m = 64.0;

        const Vector2 pos{body.position.x * px_per_m, body.position.y * px_per_m};
        const float rot = body.rotation;

        if (std::holds_alternative<RectDraw>(draw)) {
            const float w = body.width.x * px_per_m;
            const float h = body.width.y * px_per_m;

            DrawRectanglePro({-w * 0.5f, -h * 0.5f, w * 0.5f, h * 0.5f}, pos, rot, BLACK);
        } else {
            const auto texture = std::get<Texture>(draw);
            DrawTextureEx(texture, pos, rot, px_per_m, WHITE);
        }
    }
}
