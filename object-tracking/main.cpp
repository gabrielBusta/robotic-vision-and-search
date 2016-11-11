#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>

int main(int argc, char* argv[], char* envp[])
{
    cv::VideoCapture capture("walk.mpg");
    if (!capture.isOpened())
    {
        std::cout << "ERROR: FAILED TO OPEN THE INPUT FILE." << std::endl;
        return EXIT_FAILURE;
    }

    std::string window_name = "walk";
    /* create a resizable window */
    cv::namedWindow(window_name, cv::WINDOW_KEEPRATIO);

    cv::Mat frame, bw_frame;
    /* http://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html#meanstddev */
    std::vector<cv::Mat> frames;

    while (true)
    {
        capture >> frame;
        if (frame.empty())
        {
            break;
        }
        frames.push_back(frame);
        cvtColor(frame, bw_frame, cv::COLOR_BGR2GRAY);
        cv::imshow(window_name, bw_frame);
        /* wait 30 milliseconds before loading the next
           frame. This displays ~24 frames per second */
        cv::waitKey(30);
    }

    return EXIT_SUCCESS;
}
