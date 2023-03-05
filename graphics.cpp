#include "graphics.h"

namespace arkanoid_cv::impl
{
    void draw_rectangle(cv::Mat3b& window, const rect& rect, const cv::Vec3b& color);

    void draw_circle(cv::Mat3b& window, const ball& circle, const cv::Vec3b& color);

    void draw_bricks(cv::Mat3b& window, const std::vector<brick>& bricks, const cv::Vec3b& full_color, const cv::Vec3b& dying_color);
}

const cv::Mat3b& arkanoid_cv::graphics::draw_screen(const game_engine& game, const world& world)
{
    std::stringstream lives_ss;
    lives_ss << "lives " << game.get_lives();
    const auto text_size = getTextSize(lives_ss.str(), cv::FONT_HERSHEY_SIMPLEX, font_scale_, 1, nullptr);
    
    if (screen_.empty())
        screen_.create(world.size.height + text_size.height + 4, world.size.width);
    
    screen_.setTo(background_color_);

    auto game_window = cv::Mat3b{screen_, cv::Range(text_size.height + 4, screen_.rows), cv::Range(0, screen_.cols)};
    draw_world(game_window, world);

    putText(screen_, lives_ss.str(), {screen_.cols - text_size.width - 2, text_size.height + 2}, cv::FONT_HERSHEY_SIMPLEX, font_scale_, 0, 1);
    std::stringstream score_ss;
    score_ss << "score " << game.get_score();
    putText(screen_, score_ss.str(), {2, text_size.height + 2}, cv::FONT_HERSHEY_SIMPLEX, font_scale_, 0, 1);
    return screen_;
}

void arkanoid_cv::graphics::draw_world(cv::Mat3b& window, const world& world) const
{
    impl::draw_bricks(window, world.bricks, brick_initial_color_, brick_final_color_);
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

void arkanoid_cv::impl::draw_bricks(cv::Mat3b& window, const std::vector<brick>& bricks, const cv::Vec3b& full_color, const cv::Vec3b& dying_color)
{
    for (auto& brick : bricks)
    {
        const double a = (brick.lives - .99) / (brick.initial_lives - .99);
        const auto color = a * full_color + (1. - a) * dying_color;
        draw_rectangle(window, brick, color);
    }
}
