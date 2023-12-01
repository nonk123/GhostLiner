#include <algorithm>
#include <cmath>
#include <memory>

#include "camera.hpp"
#include "raymath.h"
#include "systems.hpp"
#include "thing.hpp"

#include "muli/muli.h"
#include "raylib.h"

GameState game_state = GameState::RESTART;

const double DEATH_RESTART_TIMER = 3.0;

static Vector2 last_world_pos;
static double reset_time, death_time;

void draw_things(Query<With<Body>> things) {
    BeginMode2D(camera);

    ClearBackground(RAYWHITE);

    for (const auto& thing : things) {
        const auto& body = thing->expect<Body>();

        const auto body_pos = body->rigid->GetPosition();
        const Vector2 pos{body_pos.x, body_pos.y};

        const float rot = body->rigid->GetRotation().GetAngle() * RAD2DEG;

        const float w = body->width;
        const float h = body->height;

        DrawRectanglePro(
            {pos.x, pos.y, w, h}, {w * 0.5f, h * 0.5f}, rot, BLACK
        );
    }

    EndMode2D();
}

void reset(Commands cmd, Query<All> query) {
    for (const auto& entity : query) {
        cmd.del(entity);
    }

    reset_time = GetTime();
    death_time = 0.0;
}

void ready_set_go() {
    const auto secs = GetTime() - reset_time;
    std::string text;

    if (secs <= 1.0) {
        text = "Get ready";
    } else if (secs <= 2.0) {
        text = "Set";
    } else if (secs <= 3.0) {
        text = "GOGOGO!!!";
    } else {
        game_state = GameState::PLAY;
        return;
    }

    const int font_size = 30;
    const auto text_w = MeasureText(text.c_str(), font_size);
    const auto middle = GetScreenWidth() / 2 - text_w / 2;
    DrawText(text.c_str(), middle, 10, font_size, BLACK);
}

void start(Commands cmd) {
    const auto base = new Body({0.0, 0.0}, 0.0, {1.4, 0.4}, 0.2);
    base->rigid->SetContinuous(true);

    const auto back = new Body({-0.75, -0.5}, 0.0, {0.3, 1.0}, 0.2);
    back->rigid->SetContinuous(true);

    const auto front = new Body({0.75, -0.5}, 0.0, {0.3, 1.0}, 0.2);
    front->rigid->SetContinuous(true);

    cmd.spawn(base);
    cmd.spawn(back);
    cmd.spawn(front);

    cmd.spawn(new WeldJoint(base, back, {-0.65, -0.2}));
    cmd.spawn(new WeldJoint(base, front, {0.65, -0.2}));

    const auto head = new Body({0.0, -0.7}, 0.0, {0.2, 0.2}, 0.8);
    head->rigid->SetContinuous(true);

    const auto body = new Body({0.0, -0.4}, 0.0, {0.3, 0.4}, 0.8);
    body->rigid->SetContinuous(true);

    cmd.spawn(head, new Explosive, new Player);
    cmd.spawn(body, new Explosive);
    cmd.spawn(new RevJoint(body, head, {0.0, -0.6}));

    last_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);
}

void follow_player(Query<With<Player, Body>> players) {
    for (const auto& thing : players) {
        const auto pos = thing->expect<Body>()->rigid->GetPosition();
        camera.center = {pos.x, pos.y};
    }
}

void cull_lines(Commands cmd, Query<With<Line, Body>> lines) {
    for (const auto& line : lines) {
        const auto creation_time = line->expect<Body>()->creation_time;
        const auto lifetime = GetTime() - creation_time;

        if (lifetime > 5.0) {
            cmd.del(line);
        }
    }
}

void draw_lines(Commands cmd) {
    const auto world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        const auto delta = Vector2Subtract(world_pos, last_world_pos);
        const auto length = Vector2Length(delta);

        if (length <= 0.2) {
            return;
        }

        const Vector2 mid =
            Vector2Scale(Vector2Add(world_pos, last_world_pos), 0.5);

        const float rot = std::atan2(delta.y, delta.x);

        cmd.spawn(new Line, new Body(mid, rot, {length, 0.5}, 0.1, true));
    }

    last_world_pos = world_pos;
}

void explode(
    Commands cmd, Query<With<Explosive, Body>> explosives,
    Query<With<Line, Body>> lines
) {
    // CUBIC TIME COMPLEXITY!!! YEEEEAH!!!

    const auto count = phys_world->GetContactCount();

    for (const auto& explosive : explosives) {
        const auto exp_body = explosive->expect<Body>()->rigid;

        for (const auto& line : lines) {
            const auto line_body = line->expect<Body>()->rigid;

            auto contact = phys_world->GetContacts();

            while (contact != nullptr) {
                if ((contact->GetIncidentBody() == line_body &&
                     contact->GetReferenceBody() == exp_body) ||
                    (contact->GetIncidentBody() == exp_body &&
                     contact->GetReferenceBody() == line_body)) {
                    cmd.del(explosive);
                }

                contact = contact->GetNext();
            }
        }
    }
}

void await_restart(Query<With<Player>> players) {
    if (death_time == 0.0) {
        death_time = GetTime();
    }

    for (const auto& player : players) {
        death_time = 0.0;
    }

    if (death_time == 0.0) {
        return;
    }

    if (GetTime() - death_time >= DEATH_RESTART_TIMER) {
        game_state = GameState::RESTART;
    }
}
