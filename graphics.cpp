#include "graphics.h"

namespace arkanoid_cv::impl
{
    void draw_rectangle(cv::Mat3b& window, const rect& rect);

    void draw_circle(cv::Mat3b& window, const ball& circle);

    void draw_bricks(cv::Mat3b& window, const std::vector<brick>& bricks);
}

void arkanoid_cv::draw_world(cv::Mat3b& window, const world& world, const cv::Vec3b& background_color)
{
    window.setTo(background_color);
    impl::draw_bricks(window, world.bricks);
    impl::draw_rectangle(window, world.base);
    impl::draw_circle(window, world.ball);
}

void arkanoid_cv::impl::draw_rectangle(cv::Mat3b& window, const rect& rect)
{
    rectangle(
        window,
        cv::Point2d{rect.x - rect.width / 2., rect.y - rect.height / 2.},
        cv::Point2d{rect.x + rect.width / 2., rect.y + rect.height / 2.},
        rect.color,
        -1);
}

void arkanoid_cv::impl::draw_circle(cv::Mat3b& window, const ball& circle)
{
    cv::circle(window, cv::Point2d{circle.x, circle.y}, circle.radius, circle.color, -1, cv::LINE_AA);
}

void arkanoid_cv::impl::draw_bricks(cv::Mat3b& window, const std::vector<brick>& bricks)
{
    for (auto& brick : bricks)
        draw_rectangle(window, brick);
}
