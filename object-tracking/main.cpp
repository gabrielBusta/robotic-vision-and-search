#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <iostream>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[])
{
    cv::VideoCapture capture("walk.mpg");
    if (!capture.isOpened())
    {
        std::cout << "ERROR: FAILED TO OPEN THE INPUT FILE." << std::endl;
        return EXIT_FAILURE;
    }

    cv::Mat frame; char outfile[200]; int n = 1;

    capture >> frame;
    if (frame.empty())
    {
        std::cout << "ERROR: EMPTY VIDEO FILE." << std::endl;
    }

    while (!frame.empty())
    {
        sprintf(outfile, "frame-%d.jpg", n);
        n += 1;
        imwrite(outfile, frame);
        capture >> frame;
    }

    return EXIT_SUCCESS;
}
