#pragma once
#include "objects.h"

namespace arkanoid_cv
{
    class game
    {
    public:
        explicit game(world&& world);

        [[nodiscard]] unsigned get_lives() const { return lives_; }
        [[nodiscard]] unsigned get_score() const { return score_; }
        [[nodiscard]] const world& get_world() const { return world_; }
        [[nodiscard]] bool has_lost() const { return lives_ == 0u; }
        [[nodiscard]] bool has_won() const { return world_.bricks.empty(); }
        [[nodiscard]] bool has_ended() const { return has_lost() || has_won(); }
        
        void update();

        void move_base(int x);

        void start();

    private:
        void reset();
        
        void handle_ball_collisions();
        
        world world_;
        unsigned lives_ = 3u;
        unsigned score_ = 0u;
        bool ball_on_base_ = true;
    };
}
