#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

static void detectAndDraw(const HOGDescriptor &hog, Mat &img)
{
    vector<Rect> found, found_filtered;
    // Run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
    hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);

    for(size_t i = 0; i < found.size(); i++)
    {
        Rect r = found[i];
        size_t j;
        // Do not add small detections inside a bigger detection.
        for ( j = 0; j < found.size(); j++)
        {
            if (j != i && (r & found[j]) == r)
            {
                break;
            }
        }
        if ( j == found.size() )
        {
            found_filtered.push_back(r);
        }
    }

    for (size_t i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];

        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
    }
}

int main(int argc, char* argv[])
{
    VideoCapture capture("Walk.mpg");
    if (!capture.isOpened())
    {
        cout << "ERROR: FAILED TO OPEN THE VIDEO FILE." << endl;
        return EXIT_FAILURE;
    }

    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    string windowName = "Pedestrian Detection";
    namedWindow(windowName);
    Mat frame;

    while (capture.read(frame))
    {
        if (frame.empty())
        {
            break;
        }

        Mat grayFrame;

        cvtColor(frame, grayFrame, CV_BGR2GRAY);

        detectAndDraw(hog, grayFrame);

        imshow("Pedestrian Detection", grayFrame);
        waitKey(30);
    }

    return EXIT_SUCCESS;
}
