#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[], char* envp[])
{
    cv::VideoCapture capture("Walk1.mpg");
    if (!capture.isOpened())
    {
        std::cout << "ERROR: FAILED TO OPEN THE INPUT FILE." << std::endl;
        return EXIT_FAILURE;
    }

    std::string window_name = "walk";
    /* create a resizable window */
    cv::namedWindow(window_name, cv::WINDOW_KEEPRATIO);

    cv::Mat frame, hsv_frame, mean, stddev;
    std::vector<cv::Mat> frames;

    while (true)
    {
        capture >> frame;
        if (frame.empty())
        {
            break;
        }
        frames.push_back(frame);
        cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);

        /* http://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html#meanstddev */
        cv::meanStdDev(hsv_frame, mean, stddev);

        cv::imshow(window_name, hsv_frame);
        /*
         wait 30 milliseconds before loading the next
         frame. This displays ~24 frames per second
         */
        cv::waitKey(30);
    }

    return EXIT_SUCCESS;
}
