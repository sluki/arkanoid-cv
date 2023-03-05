#pragma once
#include "objects.h"

namespace arkanoid_cv
{
    class world_builder
    {
    public:
        [[nodiscard]] world build(const cv::Size& size, unsigned brick_rows) const;

    private:
        int brick_width_ = 50;
        int brick_height_ = 20;
        int brick_padding_ = 4;

        int base_width_ = 50;
        int base_height_ = 10;

        int ball_radius_ = 6;

        int padding_sides_ = 30;
        int padding_top_ = 50;
        int padding_bottom_ = 10;
    };
}
