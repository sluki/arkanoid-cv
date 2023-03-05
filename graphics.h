#pragma once
#include <opencv2/opencv.hpp>

#include "game.h"
#include "objects.h"

namespace arkanoid_cv
{
    class graphics
    {
    public:
        const cv::Mat3b& draw_screen(const game& game);

    private:
        void draw_world(cv::Mat3b& window, const world& world) const;
        
        cv::Mat3b screen_;
        int status_height_ = 20;
        cv::Vec3b background_color_ = {200, 200, 200};
        cv::Vec3b base_color_ = {0, 255, 0};
        cv::Vec3b ball_color_ = {255, 0, 0};
        cv::Vec3b brick_initial_color_ = {222, 222, 222};
        cv::Vec3b brick_final_color_ = {0, 0, 255};
    };
}
