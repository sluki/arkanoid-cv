#include "graphics.h"

namespace arkanoid_cv::impl
{
    void draw_rectangle(cv::Mat3b& window, const rect& rect, const cv::Vec3b& color);

    void draw_circle(cv::Mat3b& window, const ball& circle, const cv::Vec3b& color);

    void draw_bricks(cv::Mat3b& window, const std::vector<brick>& bricks, const cv::Vec3b& full_color, const cv::Vec3b& dying_color);

    enum text_origin { top_left, top_right, top_center };
    void draw_text(const cv::Mat3b& screen, const std::string& text, int x, int y, text_origin origin);
}

const cv::Mat3b& arkanoid_cv::graphics::draw_screen(const game& game)
{
    const auto& world = game.get_world();
    
    if (screen_.empty())
        screen_.create(world.size.height + status_height_, world.size.width);
    
    screen_.setTo(background_color_);

    auto game_window = cv::Mat3b{screen_, cv::Range(status_height_, screen_.rows), cv::Range(0, screen_.cols)};
    draw_world(game_window, world);

    std::stringstream lives_ss;
    lives_ss << "lives " << game.get_lives();
    draw_text(screen_, lives_ss.str(), screen_.cols - 2, 2, impl::text_origin::top_right);
    
    std::stringstream score_ss;
    score_ss << "score " << game.get_score();
    draw_text(screen_, score_ss.str(), 2, 2, impl::text_origin::top_left);

    if (game.has_ended())
    {
        const std::string message = game.has_lost() ? "GAME LOST" : "GAME WON";
        draw_text(screen_, message, screen_.cols / 2, screen_.rows / 2, impl::text_origin::top_center);
    }
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

void arkanoid_cv::impl::draw_text(const cv::Mat3b& screen, const std::string& text, int x, int y, const text_origin origin)
{
    constexpr auto face = cv::FONT_HERSHEY_SIMPLEX;
    constexpr double scale = 0.5;
    constexpr int thickness = 1;
    const auto size = getTextSize(text, face, scale, thickness, nullptr);

    y += size.height;
    switch (origin)
    {
        case top_left: break;
        case top_right:
            x -= size.width;
            break;
        case top_center:
            x -= size.width / 2;
            break;
    }

    putText(screen, text, {x, y}, face, scale, 0, thickness, cv::LINE_AA);
}
