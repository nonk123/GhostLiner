#include "thing.hpp"
#include "muli/rigidbody.h"

muli::World* phys_world = nullptr;

Body::Body(
    Vector2 pos, float rotation, Vector2 size, float friction, bool _static
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

RevJoint::RevJoint(muli::RigidBody* a, muli::RigidBody* b, Vector2 anchor) {
    revolute = phys_world->CreateRevoluteJoint(a, b, {anchor.x, anchor.y});
}

WeldJoint::WeldJoint(muli::RigidBody* a, muli::RigidBody* b, Vector2 anchor) {
    weld =
        phys_world->CreateWeldJoint(a, b, {anchor.x, anchor.y}, 8.0, 1.0, 1.0);
}
