#include "game.h"


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


arkanoid_cv::game::game(world&& world): world_(std::move(world))
{
    reset();
}

void arkanoid_cv::game::handle_ball_collisions()
{
    auto closest_intersection = impl::check_intersection(world_.ball, world_.base);

    for(auto it = world_.bricks.begin(); it != world_.bricks.end();)
        if (const auto intersection = impl::check_intersection(world_.ball, *it))
        {
            if (!closest_intersection.has_value() || intersection->distance < closest_intersection->distance)
                closest_intersection = intersection;
            it->lives--;
            if (it->lives == 0)
            {
                score_ += it->initial_lives;
                it = world_.bricks.erase(it);
            }
            else
                ++it;
        }
        else
            ++it;

    if (closest_intersection)
        impl::reflect_ball(world_.ball, closest_intersection->closest_point);
}

void arkanoid_cv::game::update()
{
    if (has_ended())
        return;
    
    world_.ball.update();

    if (world_.ball.x - world_.ball.radius <= 0 || world_.ball.x + world_.ball.radius >= world_.size.width) 
        world_.ball.direction_x *= -1;
    if (world_.ball.y - world_.ball.radius <= 0)
        world_.ball.direction_y *= -1;

    if (world_.ball.y >= world_.size.height)
    {
        lives_--;
        reset();
        return;
    }

    handle_ball_collisions();
}

void arkanoid_cv::game::move_base(const int x)
{
    world_.base.x = x;
    if (ball_on_base_)
        world_.ball.x = x;
}

void arkanoid_cv::game::start()
{
    if (ball_on_base_ && !has_ended())
    {
        world_.ball.speed = 6.;
        ball_on_base_ = false;
    }
}

void arkanoid_cv::game::reset()
{
    world_.ball.x = world_.base.x;
    world_.ball.y = world_.base.y - world_.base.height / 2. - world_.ball.radius;
    world_.ball.direction_x = 0.6;
    world_.ball.direction_y = -0.8;
    world_.ball.speed = 0.;
    ball_on_base_ = true;
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
    const double old_x = ball.x - ball.direction_x * ball.speed;
    const double old_y = ball.y - ball.direction_y * ball.speed;
    
    const double distance = pow(pow(contact.x - ball.x, 2) + pow(contact.y - ball.y, 2), 0.5);

    if (distance == 0.)
    {
        ball.direction_x *= -1;
        ball.direction_y *= -1;
    }
    else
    {
        const cv::Matx21d n{
            (ball.x - contact.x) / distance,
            (ball.y - contact.y) / distance
        };
        const cv::Matx21d in{ball.direction_x, ball.direction_y};
        const auto out = in - 2 * in.dot(n) * n;
        if (abs(out(1)) < 0.5)  // ensure vertical angle is always at least 30 degrees
            {
            ball.direction_x = copysign(pow(0.75, 0.5), out(0));
            ball.direction_y = copysign(0.5, out(1));
            }
        else
        {
            ball.direction_x = out(0);
            ball.direction_y = out(1);
        }
    }

    // return ball to a "before intersection" state. This "trick" prevents repeated intersections with same objects caused by coarse sampling of time
    ball.x = old_x;
    ball.y = old_y;
}
