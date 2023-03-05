#include <opencv2/opencv.hpp>

#include "game_engine.h"
#include "objects.h"
#include "graphics.h"
#include "world_builder.h"

using namespace arkanoid_cv;


void mouse_callback(int, const int x, int, int, void* userdata)
{
    world& world = *static_cast<::world*>(userdata);

    world.base.x = x;

    if (world.ball.v < std::numeric_limits<double>::epsilon())
        world.ball.x = x;
}


int main(int, char* [])
{
    graphics graphics;
    game_engine game_engine;
    const world_builder wb;
    world world = wb.build({600, 400}, 5u);

    cv::namedWindow("arkanoid-cv", cv::WINDOW_GUI_NORMAL | cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("arkanoid-cv", mouse_callback, &world);
    while (true)
    {
        game_engine.update_world(world);
        
        imshow("arkanoid-cv", graphics.draw_screen(game_engine, world));

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
