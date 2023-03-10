#include "world_builder.h"

#include <random>

arkanoid_cv::world arkanoid_cv::world_builder::build(const cv::Size& size, const unsigned brick_rows) const
{
    world world{
        base{size.width / 2., size.height - base_height_ / 2. - padding_bottom_, base_width_, base_height_},
        ball{size.width / 2., 1. * size.height - base_height_ - ball_radius_ - padding_bottom_, ball_radius_},
        size
    };

    std::uniform_int_distribution lives_generator(1, 3);
    std::default_random_engine gen{std::random_device{}()};

    for (unsigned row = 0, y = padding_top_ + brick_height_ / 2; row < brick_rows; y += brick_height_ + brick_padding_, ++row)
        for (int x = padding_sides_ + brick_width_ / 2; x <= world.size.width - padding_sides_ - brick_width_ / 2; x += brick_width_ + brick_padding_)
            world.bricks.emplace_back(x, y, brick_width_, brick_height_, lives_generator(gen));

    return world;
}
