#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    VideoCapture capture("Walk.mpg");
    if (!capture.isOpened())
    {
        cout << "ERROR: FAILED TO OPEN THE VIDEO FILE." << endl;
        return EXIT_FAILURE;
    }

    CascadeClassifier cascade("haarcascade_fullbody.xml");
    if (cascade.empty())
    {
        cout << "ERROR: FAILED TO LOAD THE CASCADE FILE." << endl;
        return EXIT_FAILURE;
    }

    string windowName = "Pedestrian Detection";
    namedWindow(windowName);
    Mat frame, nextFrame;

    while (capture.get(CV_CAP_PROP_POS_FRAMES) <
           capture.get(CV_CAP_PROP_FRAME_COUNT) - 1)
    {
        capture.read(frame);
        if (frame.empty())
        {
            break;
        }

        Mat grayFrame;

        cvtColor(frame, grayFrame, CV_BGR2GRAY);

        capture.read(nextFrame);

        Mat grayNextFrame;
        cvtColor(nextFrame, grayNextFrame, CV_BGR2GRAY);

        Mat difference;
        absdiff(grayFrame, grayNextFrame, difference);

        Mat thresholdFrame;
        threshold(difference, thresholdFrame, 40, 255, THRESH_TOZERO);

        vector<Rect> pedestrians;
        cascade.detectMultiScale(difference, pedestrians, 1.1, 2, 0,
                                 Size(30, 30), Size(150, 150));

        for(size_t i = 0; i < pedestrians.size(); i++)
        {
            Point center(pedestrians[i].x + pedestrians[i].width * 0.5,
                         pedestrians[i].y + pedestrians[i].height * 0.5);
            ellipse(difference, center,
                    Size(pedestrians[i].width * 0.5, pedestrians[i].height * 0.5),
                    0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
        }

        imshow("Pedestrian Detection", difference);
        waitKey(30);
    }

    return EXIT_SUCCESS;
}
