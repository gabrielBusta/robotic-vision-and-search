#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

vector<Rect> detectPedestrians(Mat frame)
{
    /*
    create a histograms of oriented gradients. Then, set it up to detect
    objects using the trained people detecting support vector machine.
    */
    HOGDescriptor HOG;
    HOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    vector<Rect> pedestrians;

//    HOG.detectMultiScale(frame, pedestrians, 0, Size(4, 4),
//                         Size(32, 32), 1.05, 2);

    return pedestrians;
}

Mat drawRects(Mat frame, vector<Rect> pedestrians)
{
    Mat doctoredFrame = frame.clone();
    return doctoredFrame;
}

int main(int argc, char* argv[], char* envp[])
{
    VideoCapture capture("Walk.mpg");
    if (!capture.isOpened())
    {
        cout << "ERROR: FAILED TO OPEN THE INPUT FILE." << endl;
        return EXIT_FAILURE;
    }

    string windowName = "Pedestrian Detection";

    /* create a resizable window. */
    namedWindow(windowName, WINDOW_KEEPRATIO);

    Mat frame;
    while (capture.read(frame))
    {
        if (frame.empty())
        {
            break;
        }

        vector<Rect> pedestrians = detectPedestrians(frame);

        imshow(windowName, drawRects(frame, pedestrians));
        /*
        wait 30 milliseconds before loading and displaying
        the next frame. This displays ~24 frames per second.
        */
        waitKey(30);
    }

    return EXIT_SUCCESS;
}
