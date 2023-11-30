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
    ) {
        creation_time = GetTime();

        width = size.x;
        height = size.y;

        const auto type = _static ? muli::RigidBody::Type::static_body
                                  : muli::RigidBody::Type::dynamic_body;

        rigid = phys_world->CreateBox(width, height, type);

        rigid->SetFriction(friction);
        rigid->SetPosition({pos.x, pos.y});
        rigid->SetRotation(rotation);
    }

    ~Body() {
        if (phys_world != nullptr) {
            phys_world->Destroy(rigid);
        }
    }
};
