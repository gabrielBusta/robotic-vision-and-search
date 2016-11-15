#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const static int SENSITIVITY_VALUE = 50;
const static int BLUR_SIZE = 10;
Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
Point object = Point(0, 0);


/*
notice how we use the '&' operator for the cameraFeed. This is because
we wish to take the values passed into the function and manipulate them,
rather than just working with a copy. eg. we draw to the cameraFeed in
this function which is then displayed in the main() function.
*/
void searchForMovement(Mat thresholdImage, Mat &frame)
{
    // these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // find contours of filtered image using openCV findContours function
    findContours(thresholdImage, contours, hierarchy,
                 CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    if (!contours.empty())
    {
        /*
        the largest contour is found at the end of
        the contours vector we will simply assume that the
        biggest contour is the object we are looking for.
        */
        vector<Point> largestContour;
        largestContour = contours.at(contours.size() - 1);

        /*
        make a bounding rectangle around the largest contour then
        find its centroid this will be the object's final estimated position.
        */
        objectBoundingRectangle = boundingRect(largestContour);

        // update the objects position
        object.x = objectBoundingRectangle.x +
                   objectBoundingRectangle.width / 2;

        object.y = objectBoundingRectangle.y +
                   objectBoundingRectangle.height / 2;
    }

    // draw a green rectangle around the detected object
    rectangle(frame, Point(object.x - 25, object.y - 50),
              Point(object.x + 25, object.y + 50), Scalar(0, 255, 0));
}

int main(int argc, char* argv[])
{
    //these two can be toggled by pressing 'd' or 't'
    bool debugMode = false;
    bool trackingEnabled = false;
    //pause and resume code
    bool pause = false;
    //set up the matrices that we will need
    //the two frames we will be comparing
    Mat frame, nextFrame;
    //their grayscale images (needed for absdiff() function)
    Mat frameGray, nextFrameGray;
    //resulting difference image
    Mat frameDifference;
    //thresholded difference image (for use in findContours() function)
    Mat frameThreshold;

    //video capture object.
    VideoCapture capture("Walk.mpg");
    if(!capture.isOpened())
    {
        cout << "ERROR: FAILED TO OPEN THE INPUT FILE." << endl;
        return EXIT_FAILURE;
    }

    //check if the video has reach its last frame.
    //we add '-1' because we are reading two frames from the video at a time.
    //if this is not included, we get a memory error!
    while(capture.get(CV_CAP_PROP_POS_FRAMES) <
          capture.get(CV_CAP_PROP_FRAME_COUNT) - 1)
    {
        capture.read(frame);
        if (frame.empty())
        {
            break;
        }

        //convert frame to gray scale for frame differencing
        cv::cvtColor(frame, frameGray, COLOR_BGR2GRAY);

        //copy second frame
        capture.read(nextFrame);

        //convert nextFrame to gray scale for frame differencing
        cv::cvtColor(nextFrame,nextFrameGray,COLOR_BGR2GRAY);

        //perform frame differencing with the sequential images. This will output an "intensity image"
        //do not confuse this with a threshold image, we will need to perform thresholding afterwards.
        cv::absdiff(frameGray,nextFrameGray,frameDifference);

        //threshold intensity image at a given sensitivity value
        cv::threshold(frameDifference, frameThreshold, SENSITIVITY_VALUE, 255, THRESH_BINARY);

        if(debugMode==true)
        {
            //show the difference image and threshold image
            cv::imshow("Difference Image", frameDifference);
            cv::imshow("Threshold Image", frameThreshold);
        }
        else
        {
            //if not in debug mode, destroy the windows so we don't see them anymore
            cv::destroyWindow("Difference Image");
            cv::destroyWindow("Threshold Image");

        }
        //use blur() to smooth the image, remove possible noise and
        //increase the size of the object we are trying to track. (Much like dilate and erode)
        cv::blur(frameThreshold, frameThreshold, cv::Size(BLUR_SIZE,BLUR_SIZE));

        //threshold again to obtain binary image from blur output
        cv::threshold(frameThreshold,frameThreshold,SENSITIVITY_VALUE,255,THRESH_BINARY);


        if(debugMode==true)
        {
            //show the threshold image after it's been "blurred"
            imshow("Final Threshold Image", frameThreshold);

        }
        else
        {
            //if not in debug mode, destroy the windows so we don't see them anymore
            cv::destroyWindow("Final Threshold Image");

        }

        /// ---------- NEED TRACKING METHOD FOR HUMAN BODY
        //if tracking enabled, search for contours in our thresholded image
        if(trackingEnabled)
        {
            searchForMovement(frameThreshold, frame);
        }

        //show our captured frame
        imshow("frame",frame);
        //check to see if a button has been pressed.
        //this 10ms delay is necessary for proper operation of this program
        //if removed, frames will not have enough time to referesh and a blank
        //image will appear.
        switch(waitKey(50))
        {

        case 27: //'esc' key has been pressed, exit program.
            return 0;
        case 116: //'t' has been pressed. this will toggle tracking
            trackingEnabled = !trackingEnabled;
            if (trackingEnabled == false) cout<<"Tracking disabled."<<endl;
            else
                cout <<"Tracking enabled."<<endl;
            break;
        case 100: //'d' has been pressed. this will debug mode
            debugMode = !debugMode;
            if(debugMode == false) cout<<"Debug mode disabled."<<endl;
            else
                cout <<"Debug mode enabled."<<endl;
            break;
        case 112: //'p' has been pressed. this will pause/resume the code.
            pause = !pause;
            if(pause == true)
            {
                cout<<"Code paused, press 'p' again to resume"<<endl;
                while (pause == true)
                {
                    //stay in this loop until
                    switch (waitKey())
                    {
                    //a switch statement inside a switch statement? Mind blown.
                    case 112:
                        //change pause back to false
                        pause = false;
                        cout<<"Code resumed."<<endl;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
