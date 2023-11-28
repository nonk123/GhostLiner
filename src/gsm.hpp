#pragma once

#include <memory>
#include <vector>

#include "raylib.h"

struct Exit {};

class GameState;

extern struct GSM {
    std::vector<std::unique_ptr<GameState>> stack;

    GSM();

    void tick();
} gsm;

class GameState {
    friend GSM;

  protected:
    struct {
        friend GSM;

      private:
        GameState *_replacement = nullptr, *_push = nullptr;
        bool _pop = false;

      public:
        void pop() {
            _pop = true;
        }

        void push(GameState* next) {
            if (_push != nullptr) {
                delete _push;
            }

            _push = next;
        }

        void replace(GameState* next) {
            if (_replacement != nullptr) {
                _replacement = nullptr;
            }

            _replacement = next;
        }
    } transition;

  public:
    virtual void init() {}
    virtual void update() = 0;
    virtual void overlay() {}

    virtual ~GameState() = default;
};

class Restart : public GameState {
  private:
    double init_time;

  public:
    Restart() {}

    void init() override;
    void update() override;
    void overlay() override;
};

class Play : public GameState {
  private:
    Vector2 last_world_pos;

  public:
    Play() {}

    void init() override;
    void update() override;
};
