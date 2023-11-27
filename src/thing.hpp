#pragma once

#include <memory>
#include <variant>
#include <vector>

#include "box2d-lite/Body.h"
#include "box2d-lite/World.h"
#include "raylib.h"

namespace Things {
    extern World phys_world;
};

struct RectDraw {};

struct BodyExt : public Body {
    std::variant<RectDraw, Texture> draw = RectDraw();

    BodyExt() = default;
    ~BodyExt() = default;
};

struct Thing {
    BodyExt body;
};

namespace Things {
    using List = std::vector<std::unique_ptr<Thing>>;
    extern List all;

    void spawn(Thing*);
    void erase(std::size_t);
    void clear();

    void update();
    void draw();
} // namespace Things
