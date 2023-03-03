#include <opencv2/opencv.hpp>


struct object
{
    object(const double x, const double y, const cv::Vec3b& color)
        : x(x),
          y(y),
          color(color)
    {
    }

    double x;
    double y;
    cv::Vec3b color;
};


struct rect : object
{
    rect(const double x, const double y, const int width, const int height, const cv::Vec3b& color)
        : object(x, y, color),
          width(width),
          height(height)
    {
    }

    int width;
    int height;
};


struct ball : object
{
    ball(const double x, const double y, const int radius, const cv::Vec3b& color)
        : object(x, y, color),
          radius(radius)
    {
    }

    int radius;
    double v = 0.;
    double dx = 0.6;
    double dy = -0.8;
};


struct brick : rect
{
    brick(const int x, const int y, const int width, const int height, const cv::Vec3b& color)
        : rect(x, y, width, height, color)
    {
    }
};


struct base : rect
{
    base(const int x, const int y, const int width, const int height, const cv::Vec3b& color)
        : rect(x, y, width, height, color)
    {
    }
};


struct world
{
    world(base base, ball ball, cv::Size size)
        : base(std::move(base)),
          ball(std::move(ball)),
          size(std::move(size))
    {
    }

    std::vector<brick> bricks;
    base base;
    ball ball;
    cv::Size size;
};


void draw_rectangle(cv::Mat3b& window, const rect& rect)
{
    rectangle(
        window,
        cv::Point2d{rect.x - rect.width / 2., rect.y - rect.height / 2.},
        cv::Point2d{rect.x + rect.width / 2., rect.y + rect.height / 2.},
        rect.color,
        -1);
}


void draw_circle(cv::Mat3b& window, const ball& circle)
{
    cv::circle(window, cv::Point2d{circle.x, circle.y}, circle.radius, circle.color, -1, cv::LINE_AA);
}


void draw_bricks(cv::Mat3b& window, const std::vector<brick>& bricks)
{
    for (auto& brick : bricks)
        draw_rectangle(window, brick);
}


bool check_intersection(const ball& ball, const rect& rect)
{
    if (ball.x + ball.radius < rect.x - rect.width / 2.)
        return false;
    if (ball.x - ball.radius > rect.x + rect.width / 2.)
        return false;
    if (ball.y + ball.radius < rect.y - rect.height / 2.)
        return false;
    if (ball.y - ball.radius > rect.y + rect.height / 2.)
        return false;
    return true;
}


void update_world(world& world, const double dt = 1.)
{
    world.ball.x += world.ball.dx * dt * world.ball.v;
    world.ball.y += world.ball.dy * dt * world.ball.v;

    if (world.ball.x <= 0 || world.ball.x >= world.size.width) 
        world.ball.dx *= -1;
    if (world.ball.y <= 0 || world.ball.y >= world.size.height)
        world.ball.dy *= -1;

    if (check_intersection(world.ball, world.base))
        world.ball.dy *= -1;

    const auto it = std::ranges::find_if(
        world.bricks,
        [&](const brick& b) { return check_intersection(world.ball, b);}
        );
    
    if (it != world.bricks.end())
    {
        world.bricks.erase(it);
        world.ball.dy *= -1;
    }
}


void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
    world& world = *static_cast<::world*>(userdata);

    world.base.x = x;
}


int main(int, char* [])
{
    world world{
        base{300, 380, 50, 10, {0, 255, 0}},
        ball{300, 368, 6, {255, 0, 0}},
        cv::Size{600, 400}
    };
    
    cv::Mat3b window{world.size};

    for (int y = 60; y < 150; y += 20)
    for (int x = 24; x < window.cols - 20; x += 44)
    {
        world.bricks.emplace_back(x, y, 40, 16, cv::Vec3b{0, 0, 200});
    }

    cv::namedWindow("arkanoid-cv", cv::WINDOW_GUI_NORMAL | cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("arkanoid-cv", mouse_callback, &world);
    while (true)
    {
        // update objects
        update_world(world);

        // draw objects
        window.setTo(cv::Vec3b{200, 200, 200});
        draw_bricks(window, world.bricks);
        draw_rectangle(window, world.base);
        draw_circle(window, world.ball);
        
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
