#include <opencv2/opencv.hpp>


struct rect
{
    rect(const int x, const int y, const int width, const int height, const cv::Vec3b& color)
        : x(x), y(y), width(width), height(height), color(color)
    {
    }

    int x;
    int y;
    int width;
    int height;
    cv::Vec3b color;
};


struct circle
{
    circle(const int x, const int y, const int radius, const cv::Vec3b& color)
        : x(x), y(y), radius(radius), color(color)
    {
    }

    int x;
    int y;
    int radius;
    cv::Vec3b color;
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


void draw_rectangle(cv::Mat3b& window, const rect& rect)
{
    rectangle(
        window,
        cv::Point{rect.x - rect.width / 2, rect.y - rect.height / 2},
        cv::Point{rect.x + rect.width / 2, rect.y + rect.height / 2},
        rect.color,
        -1);
}


void draw_circle(cv::Mat3b& window, const circle& circle)
{
    cv::circle(window, cv::Point{circle.x, circle.y}, circle.radius, circle.color, -1, cv::LINE_AA);
}


void draw_bricks(cv::Mat3b& window, const std::vector<brick>& bricks)
{
    for (auto& brick : bricks)
        draw_rectangle(window, brick);
}


int main(int, char* [])
{
    cv::Mat3b window(400, 600);

    std::vector<brick> bricks;
    for (int y = 60; y < 150; y += 10)
    for (int x = 24; x < window.cols - 20; x += 22)
    {
        bricks.emplace_back(x, y, 20, 8, cv::Vec3b{0, 0, 200});
    }

    const base base{300, 380, 50, 10, {0, 255, 0}};
    const circle ball{300, 368, 6, {255, 0, 0}};

    while (true)
    {
        // update objects

        // draw objects
        window.setTo(cv::Vec3b{200, 200, 200});
        draw_bricks(window, bricks);
        draw_rectangle(window, base);
        draw_circle(window, ball);
        
        imshow("arkanoid-cv", window);

        const auto key = cv::waitKey(5);

        // process input
        if (key == 27)
            break;
    }
    
    return 0;
}
