#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <vector>

std::string windowName = "Pedestrian Detection";
cv::HOGDescriptor HOG;

std::vector<cv::Rect> detect(cv::Mat frame)
{
    std::vector<cv::Rect> people;
    return people;
}

int main(int argc, char* argv[], char* envp[])
{
    cv::VideoCapture capture("Walk.mpg");
    if (!capture.isOpened())
    {
        std::cout << "ERROR: FAILED TO OPEN THE INPUT FILE." << std::endl;
        return EXIT_FAILURE;
    }

    /* create a resizable window */
    cv::namedWindow(windowName, cv::WINDOW_KEEPRATIO);

    /*
    set the histograms of oriented gradients to detect objects
    using the trained people detecting SVM
    */
    HOG.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    cv::Mat frame;

    while (capture.read(frame))
    {
        if (frame.empty())
        {
            break;
        }

        cv::imshow(windowName, frame);
        /*
        wait 30 milliseconds before loading and displaying
        the next frame. This displays ~24 frames per second
        */
        cv::waitKey(30);
    }

    return EXIT_SUCCESS;
}
