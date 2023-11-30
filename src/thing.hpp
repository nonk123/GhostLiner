#pragma once

#include <memory>
#include <set>
#include <variant>

#include "muli/muli.h"
#include "muli/rigidbody.h"
#include "raylib.h"

namespace Things {
    extern muli::World* phys_world;
}

struct RectDraw {};

enum Tag {
    DELETE_ME,
    PLAYER,
    LINE,
};

struct Thing {
    using Draw = std::variant<RectDraw, Texture>;
    Draw draw;

    float width, height;
    muli::RigidBody* body = nullptr;

    std::set<Tag> tags;

    double creation_time = 0;

    Thing(float width, float height, Draw draw)
        : draw(draw), width(width), height(height) {}

    Thing(float width, float height) : Thing(width, height, RectDraw()) {}

    ~Thing() {
        if (Things::phys_world != nullptr) {
            Things::phys_world->Destroy(body);
        }
    }
};

namespace Things {
    using List = std::vector<std::shared_ptr<Thing>>;
    extern List all;

    template <typename... Args>
    std::weak_ptr<Thing> spawn_ex(bool dynamic, Args&&... args) {
        const auto thing = std::make_shared<Thing>(args...);
        thing->creation_time = GetTime();

        const auto type = dynamic ? muli::RigidBody::Type::dynamic_body
                                  : muli::RigidBody::Type::static_body;

        const auto body =
            phys_world->CreateBox(thing->width, thing->height, type);
        thing->body = body;

        const std::weak_ptr<Thing> weak = thing;
        all.push_back(std::move(thing));
        return weak;
    }

    template <typename... Args>
    std::weak_ptr<Thing> spawn_dynamic(Args&&... args) {
        return spawn_ex(true, args...);
    }

    template <typename... Args>
    std::weak_ptr<Thing> spawn_static(Args&&... args) {
        return spawn_ex(false, args...);
    }

    void erase(std::size_t);
    void clear();

    void update();
    void draw();
} // namespace Things
