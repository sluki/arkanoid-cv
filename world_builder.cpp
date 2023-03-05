#include "world_builder.h"

arkanoid_cv::world arkanoid_cv::world_builder::build(const cv::Size& size, const unsigned brick_rows) const
{
    world world{
        base{size.width / 2., size.height - base_height_ / 2. - padding_, base_width_, base_height_},
        ball{size.width / 2., 1. * size.height - base_height_ - ball_radius_ - padding_, ball_radius_},
        size
    };

    for (unsigned row = 0, y = padding_top_ + brick_height_ / 2; row < brick_rows; y += brick_height_ + brick_padding_, ++row)
        for (int x = padding_ + brick_width_ / 2; x <= world.size.width - padding_ - brick_width_ / 2; x += brick_width_ + brick_padding_)
        {
            world.bricks.emplace_back(x, y, brick_width_, brick_height_);
        }

    return world;
}
