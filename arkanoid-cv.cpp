#include <opencv2/opencv.hpp>

#include "game.h"
#include "graphics.h"
#include "world_builder.h"

using namespace arkanoid_cv;


void mouse_callback(int, const int x, int, int, void* userdata)
{
    static_cast<::game*>(userdata)->move_base(x);
}


int main(int, char* [])
{
    graphics graphics;
    const world_builder wb;
    game game{wb.build({600, 400}, 5u)};

    cv::namedWindow("arkanoid-cv", cv::WINDOW_GUI_NORMAL | cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("arkanoid-cv", mouse_callback, &game);
    while (true)
    {
        game.update();
        imshow("arkanoid-cv", graphics.draw_screen(game));
        const int key = cv::waitKey(5);

        // process input
        if (key == 27)
            break;
        if (key == 32)
            game.start();
        else if (key != -1)
            std::cout << "Unknown key pressed: " << key << ". Press Esc to quit and Space to start." << std::endl;
    }
    
    return 0;
}
