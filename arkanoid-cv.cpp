#include <opencv2/opencv.hpp>
#include "objects.h"
#include "graphics.h"
#include "world_builder.h"

using namespace arkanoid_cv;


struct intersection
{
    cv::Point2d closest_point;
    double distance;
};


std::optional<intersection> check_intersection(const ball& ball, const rect& rect)
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


void reflect_ball(ball& ball, const cv::Point2d& contact)
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


void update_world(world& world)
{
    world.ball.x += world.ball.dx * world.ball.v;
    world.ball.y += world.ball.dy * world.ball.v;

    if (world.ball.x <= 0 || world.ball.x >= world.size.width) 
        world.ball.dx *= -1;
    if (world.ball.y <= 0 || world.ball.y >= world.size.height)
        world.ball.dy *= -1;

    if (check_intersection(world.ball, world.base))
        world.ball.dy = -abs(world.ball.dy);  // base always reflects upwards; this prevents issues such as ball entering base

    std::optional<intersection> closest_intersection;

    for(auto it = world.bricks.begin(); it != world.bricks.end();)
    {
        if (const auto intersection = check_intersection(world.ball, *it))
        {
            if (!closest_intersection.has_value() || intersection->distance < closest_intersection->distance)
                closest_intersection = intersection;
            it = world.bricks.erase(it);
        }
        else
            ++it;
    }

    if (closest_intersection)
        reflect_ball(world.ball, closest_intersection->closest_point);
}


void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
    world& world = *static_cast<::world*>(userdata);

    world.base.x = x;

    if (world.ball.v < std::numeric_limits<double>::epsilon())
        world.ball.x = x;
}


int main(int, char* [])
{
    const world_builder wb;
    const graphics graphics;
    world world = wb.build({600, 400}, 5u);
    
    cv::Mat3b window{world.size};

    cv::namedWindow("arkanoid-cv", cv::WINDOW_GUI_NORMAL | cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("arkanoid-cv", mouse_callback, &world);
    while (true)
    {
        // update objects
        update_world(world);

        // draw objects
        graphics.draw_world(window, world);
        
        imshow("arkanoid-cv", window);

        const int key = cv::waitKey(5);

        // process input
        
        if (key == 27)
            break;
        if (key == 32)
        {
            world.ball.v = 6.;
        }
        else if (key != -1)
        {
            std::cout << key << std::endl;
        }
    }
    
    return 0;
}
