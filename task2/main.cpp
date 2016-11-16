#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void findObject(Mat, Mat &, Point &);
void drawObjectBoundingRectangle(Mat, Point);

int main(int argc, char* argv[])
{
    VideoCapture capture("Walk.mpg");

    if(!capture.isOpened())
    {
        cerr << "FAILED TO OPEN THE INPUT VIDEO FILE FOR READING." << endl;
        return EXIT_FAILURE;
    }

    int codec = -1;
    double fps = capture.get(CV_CAP_PROP_FPS);
    Size frameSize = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),
                          (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    bool isColor = true;

    VideoWriter writer("output.avi", codec, fps, frameSize, isColor);

    if (!writer.isOpened())
    {
        cerr << "FAILED TO OPEN THE OUTPUT VIDEO FILE FOR WRITING." << endl;
        return EXIT_FAILURE;
    }

    const int frameCount = capture.get(CV_CAP_PROP_FRAME_COUNT);
    const int MIN_THRESH = 50, MAX_VAL = 255;
    const Size BLUR_SIZE = Size(10, 10);

    Mat frame, nextFrame,
        frameGray, nextFrameGray,
        frameDifference,
        frameThreshold;

    Point objectPosition = Point(0, 0);

    while(capture.get(CV_CAP_PROP_POS_FRAMES) < frameCount - 1)
    {
        capture.read(frame);

        if (frame.empty())
        {
            break;
        }

        capture.read(nextFrame);

        if (nextFrame.empty())
        {
            break;
        }

        cvtColor(frame, frameGray, COLOR_BGR2GRAY);
        cvtColor(nextFrame, nextFrameGray, COLOR_BGR2GRAY);

        absdiff(frameGray, nextFrameGray, frameDifference);

        threshold(frameDifference, frameThreshold,
                  MIN_THRESH, MAX_VAL, THRESH_BINARY);

        /*
        use blur() to smooth the image, remove possible noise and
        increase the size of the object we are trying to track.
        */
        blur(frameThreshold, frameThreshold, BLUR_SIZE);

        threshold(frameThreshold, frameThreshold,
                  MIN_THRESH, MAX_VAL, THRESH_BINARY);

        findObject(frameThreshold, frame, objectPosition);

        drawObjectBoundingRectangle(frame, objectPosition);
        drawObjectBoundingRectangle(nextFrame, objectPosition);

        writer.write(frame);
        writer.write(nextFrame);
    }
    return EXIT_SUCCESS;
}

void findObject(Mat frameThreshold, Mat &frame, Point &objectPosition)
{
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(frameThreshold, contours, hierarchy,
                 CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    if (!contours.empty())
    {
        vector<Point> largestContour;
        largestContour = contours.at(contours.size() - 1);

        Rect contourBoundingRectangle = boundingRect(largestContour);

        objectPosition.x = contourBoundingRectangle.x +
                           contourBoundingRectangle.width / 2;

        objectPosition.y = contourBoundingRectangle.y +
                           contourBoundingRectangle.height / 2;
    }
}

void drawObjectBoundingRectangle(Mat frame, Point objectPosition)
{
    int thickness = 3;
    Scalar color = Scalar(0, 255, 0);

    rectangle(frame,
              Point(objectPosition.x - 40, objectPosition.y - 55),
              Point(objectPosition.x + 40, objectPosition.y + 55),
              color, thickness);
}
