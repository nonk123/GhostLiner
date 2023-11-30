#pragma once

#include <memory>
#include <set>
#include <variant>

#include "SECS.hpp"
#include "muli/muli.h"
#include "raylib.h"

extern muli::World* phys_world;

struct Player : public Component {};
struct Line : public Component {};

struct Body : public Component {
    muli::RigidBody* rigid = nullptr;

    float width, height;
    double creation_time;

    Body(
        Vector2 pos, float rotation, Vector2 size, float friction,
        bool _static = false
    );

    ~Body() {
        if (phys_world != nullptr) {
            phys_world->Destroy(rigid);
        }
    }
};
