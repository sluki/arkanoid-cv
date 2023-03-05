#pragma once
#include <opencv2/opencv.hpp>
#include "objects.h"

namespace arkanoid_cv
{
    void draw_world(cv::Mat3b& window, const world& world, const cv::Vec3b& background_color);
}
