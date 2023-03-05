#pragma once
#include <opencv2/opencv.hpp>

namespace arkanoid_cv
{
    struct object
    {
        object(const double x, const double y)
            : x(x),
              y(y)
        {
        }

        double x;
        double y;
    };

    struct rect : object
    {
        rect(const double x, const double y, const int width, const int height)
            : object(x, y),
              width(width),
              height(height)
        {
        }

        [[nodiscard]] double left() const { return x - width / 2.; }
        [[nodiscard]] double right() const { return x + width / 2.; }
        [[nodiscard]] double top() const { return y - height / 2.; }
        [[nodiscard]] double bottom() const { return y + height / 2.; }

        int width;
        int height;
    };

    struct ball : object
    {
        ball(const double x, const double y, const int radius)
            : object(x, y),
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
        brick(const double x, const double y, const int width, const int height)
            : rect(x, y, width, height)
        {
        }
    };


    struct base : rect
    {
        base(const double x, const double y, const int width, const int height)
            : rect(x, y, width, height)
        {
        }
    };

    
    struct world
    {
        world(const base base, const ball ball, const cv::Size size)
            : base(base),
              ball(ball),
              size(size)
        {
        }

        std::vector<brick> bricks;
        base base;
        ball ball;
        cv::Size size;
    };
}
