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
struct Explosive : public Component {};

template <typename Base, typename T>
void destroy_if_valid(T* item, Base* source) {
    while (source != nullptr) {
        if (source == item) {
            phys_world->Destroy(item);
            return;
        }

        source = source->GetNext();
    }
}

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
            destroy_if_valid(rigid, phys_world->GetBodyList());
        }
    }
};

struct RevJoint : public Component {
    muli::RevoluteJoint* revolute;

    RevJoint(muli::RigidBody*, muli::RigidBody*, Vector2);

    RevJoint(Body* a, Body* b, Vector2 v) : RevJoint(a->rigid, b->rigid, v) {}

    ~RevJoint() {
        if (phys_world != nullptr) {
            destroy_if_valid(revolute, phys_world->GetJoints());
        }
    }
};

struct WeldJoint : public Component {
    muli::WeldJoint* weld;

    WeldJoint(muli::RigidBody*, muli::RigidBody*, Vector2);

    WeldJoint(Body* a, Body* b, Vector2 v) : WeldJoint(a->rigid, b->rigid, v) {}

    ~WeldJoint() {
        if (phys_world != nullptr) {
            destroy_if_valid(weld, phys_world->GetJoints());
        }
    }
};
