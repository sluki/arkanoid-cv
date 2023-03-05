#include "graphics.h"

namespace arkanoid_cv::impl
{
    void draw_rectangle(cv::Mat3b& window, const rect& rect, const cv::Vec3b& color);

    void draw_circle(cv::Mat3b& window, const ball& circle, const cv::Vec3b& color);

    void draw_bricks(cv::Mat3b& window, const std::vector<brick>& bricks, const cv::Vec3b& color);
}

void arkanoid_cv::graphics::draw_world(cv::Mat3b& window, const world& world) const
{
    window.setTo(background_color_);
    impl::draw_bricks(window, world.bricks, brick_color_);
    impl::draw_rectangle(window, world.base, base_color_);
    impl::draw_circle(window, world.ball, ball_color_);
}

void arkanoid_cv::impl::draw_rectangle(cv::Mat3b& window, const rect& rect, const cv::Vec3b& color)
{
    rectangle(
        window,
        cv::Point2d{rect.x - rect.width / 2., rect.y - rect.height / 2.},
        cv::Point2d{rect.x + rect.width / 2., rect.y + rect.height / 2.},
        color,
        -1);
}

void arkanoid_cv::impl::draw_circle(cv::Mat3b& window, const ball& circle, const cv::Vec3b& color)
{
    cv::circle(window, cv::Point2d{circle.x, circle.y}, circle.radius, color, -1, cv::LINE_AA);
}

void arkanoid_cv::impl::draw_bricks(cv::Mat3b& window, const std::vector<brick>& bricks, const cv::Vec3b& color)
{
    for (auto& brick : bricks)
        draw_rectangle(window, brick, color);
}
