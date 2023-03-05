#pragma once
#include "objects.h"

namespace arkanoid_cv
{
    class game_engine
    {
    public:
        [[nodiscard]] unsigned get_lives() const { return lives_; }
        [[nodiscard]] unsigned get_score() const { return score_; }
        
        void update_world(world& world);

    private:
        unsigned lives_ = 3u;
        unsigned score_ = 0u;
    };
}
