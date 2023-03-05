#pragma once
#include <opencv2/opencv.hpp>

namespace arkanoid_cv
{
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

        [[nodiscard]] double left() const { return x - width / 2.; }
        [[nodiscard]] double right() const { return x + width / 2.; }
        [[nodiscard]] double top() const { return y - height / 2.; }
        [[nodiscard]] double bottom() const { return y + height / 2.; }

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
        world(base base, ball ball, const cv::Size size)
            : base(std::move(base)),
              ball(std::move(ball)),
              size(size)
        {
        }

        std::vector<brick> bricks;
        base base;
        ball ball;
        cv::Size size;
    };
}
