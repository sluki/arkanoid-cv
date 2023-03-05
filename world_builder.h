#pragma once
#include "objects.h"

namespace arkanoid_cv
{
    class world_builder
    {
    public:
        [[nodiscard]] world build(const cv::Size& size, unsigned brick_rows) const;

    private:
        int brick_width_ = 40;
        int brick_height_ = 16;
        int brick_padding_ = 4;

        int base_width_ = 50;
        int base_height_ = 10;

        int ball_radius_ = 6;

        int padding_ = 10;
        int padding_top_ = 60;
    };
}
