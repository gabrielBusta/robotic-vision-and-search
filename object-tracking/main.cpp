#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <vector>

std::vector<cv::Rect> detectPedestrians(cv::Mat frame, cv::HOGDescriptor HOG)
{
    std::vector<cv::Rect> pedestrians;
    return pedestrians;
}

cv::Mat drawRects(cv::Mat frame, std::vector<cv::Rect> pedestrians)
{
    cv::Mat doctoredFrame = frame.clone();
    return doctoredFrame;
}

int main(int argc, char* argv[], char* envp[])
{
    cv::VideoCapture capture("Walk.mpg");
    if (!capture.isOpened())
    {
        std::cout << "ERROR: FAILED TO OPEN THE INPUT FILE." << std::endl;
        return EXIT_FAILURE;
    }

    std::string windowName = "Pedestrian Detection";
    /* create a resizable window. */
    cv::namedWindow(windowName, cv::WINDOW_KEEPRATIO);

    /*
    create a histograms of oriented gradients. Then, set it up to detect
    objects using the trained people detecting support vector machine.
    */
    cv::HOGDescriptor HOG;
    HOG.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    cv::Mat frame;
    std::vector<cv::Rect> pedestrians;

    while (capture.read(frame))
    {
        if (frame.empty())
        {
            break;
        }

        pedestrians = detectPedestrians(frame, HOG);

        cv::imshow(windowName, drawRects(frame, pedestrians));
        /*
        wait 30 milliseconds before loading and displaying
        the next frame. This displays ~24 frames per second.
        */
        cv::waitKey(30);
    }

    return EXIT_SUCCESS;
}
