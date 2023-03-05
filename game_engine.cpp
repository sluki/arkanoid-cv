#include "game_engine.h"


namespace arkanoid_cv::impl
{
    struct intersection
    {
        cv::Point2d closest_point;
        double distance;
    };

    std::optional<intersection> check_intersection(const ball& ball, const rect& rect);

    void reflect_ball(ball& ball, const cv::Point2d& contact);
}


void arkanoid_cv::game_engine::update_world(world& world)
{
    const double initial_ball_x = world.ball.x;
    const double initial_ball_y = world.ball.y;
    world.ball.x += world.ball.dx * world.ball.v;
    world.ball.y += world.ball.dy * world.ball.v;

    if (world.ball.x <= 0 || world.ball.x >= world.size.width) 
        world.ball.dx *= -1;
    if (world.ball.y <= 0 || world.ball.y >= world.size.height)
        world.ball.dy *= -1;

    auto closest_intersection = impl::check_intersection(world.ball, world.base);

    for(auto it = world.bricks.begin(); it != world.bricks.end();)
    {
        if (const auto intersection = impl::check_intersection(world.ball, *it))
        {
            if (!closest_intersection.has_value() || intersection->distance < closest_intersection->distance)
                closest_intersection = intersection;
            it->lives--;
            if (it->lives == 0)
                it = world.bricks.erase(it);
            else
                ++it;
        }
        else
            ++it;
    }

    if (closest_intersection)
    {
        impl::reflect_ball(world.ball, closest_intersection->closest_point);
        world.ball.x = initial_ball_x;
        world.ball.y = initial_ball_y;
    }
}

std::optional<arkanoid_cv::impl::intersection> arkanoid_cv::impl::check_intersection(const ball& ball, const rect& rect)
{
    double intersection_x = ball.x;
    double intersection_y = ball.y;
    if (ball.x < rect.left()) intersection_x = rect.left();
    else if (ball.x > rect.right()) intersection_x = rect.right();
    if (ball.y < rect.top()) intersection_y = rect.top();
    else if (ball.y > rect.bottom()) intersection_y = rect.bottom();

    const double sqr_distance = pow(intersection_x - ball.x, 2) + pow(intersection_y - ball.y, 2);
    if (sqr_distance <= ball.radius * ball.radius)
        return intersection{cv::Point2d{intersection_x, intersection_y}, pow(sqr_distance, 0.5)};
    return std::nullopt;
}

void arkanoid_cv::impl::reflect_ball(ball& ball, const cv::Point2d& contact)
{
    const double distance = pow(pow(contact.x - ball.x, 2) + pow(contact.y - ball.y, 2), 0.5);

    if (distance == 0.)
    {
        ball.dx *= -1;
        ball.dy *= -1;
    }
    else
    {
        const cv::Matx21d n{
            (ball.x - contact.x) / distance,
            (ball.y - contact.y) / distance
        };
        const cv::Matx21d in{ball.dx, ball.dy};
        const auto out = in - 2 * in.dot(n) * n;
        if (abs(out(1)) < 0.5)  // ensure vertical angle is always at least 30 degrees
            {
            ball.dx = copysign(pow(0.75, 0.5), out(0));
            ball.dy = copysign(0.5, out(1));
            }
        else
        {
            ball.dx = out(0);
            ball.dy = out(1);
        }
    }   
}
