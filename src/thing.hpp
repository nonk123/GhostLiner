#pragma once

#include <memory>
#include <variant>
#include <vector>

#include "muli/muli.h"
#include "muli/rigidbody.h"
#include "raylib.h"

struct RectDraw {};

struct Thing {
    using Draw = std::variant<RectDraw, Texture>;
    Draw draw;

    float width, height;
    muli::RigidBody* underlying_body = nullptr;

    Thing(float width, float height, Draw draw) : draw(draw), width(width), height(height) {}
    Thing(float width, float height) : Thing(width, height, RectDraw()) {}

    muli::RigidBody* operator->() {
        return underlying_body;
    }

    const muli::RigidBody* operator->() const {
        return underlying_body;
    }
};

namespace Things {
    extern muli::World* phys_world;

    using List = std::vector<Thing>;
    extern List all;

    template <typename... Args>
    Thing& spawn_ex(bool dynamic, Args&&... args) {
        Thing thing(args...);

        const auto type = dynamic ? muli::RigidBody::Type::dynamic_body : muli::RigidBody::Type::static_body;

        const auto body = phys_world->CreateBox(thing.width, thing.height, type);
        thing.underlying_body = body;

        all.push_back(std::move(thing));
        return all.back();
    }

    template <typename... Args>
    Thing& spawn_dynamic(Args&&... args) {
        return spawn_ex(true, args...);
    }

    template <typename... Args>
    Thing& spawn_static(Args&&... args) {
        return spawn_ex(false, args...);
    }

    void erase(std::size_t);
    void clear();

    void update();
    void draw();
} // namespace Things
