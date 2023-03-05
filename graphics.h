#pragma once
#include <opencv2/opencv.hpp>
#include "objects.h"

namespace arkanoid_cv
{
    class graphics
    {
    public:
        void draw_world(cv::Mat3b& window, const world& world) const;

    private:
        cv::Vec3b background_color_ = {200, 200, 200};
        cv::Vec3b base_color_ = {0, 255, 0};
        cv::Vec3b ball_color_ = {255, 0, 0};
        cv::Vec3b brick_color_ = {0, 0, 200};
    };
}
