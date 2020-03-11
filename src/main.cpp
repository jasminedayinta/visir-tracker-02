#include "types.h"
#include <iostream>
#include <time.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/objdetect.hpp"

/**Code is based on the following websites: https://docs.opencv.org/master/d4/dee/tutorial_optical_flow.html
   https://docs.opencv.org/master/d4/d8c/tutorial_py_shi_tomasi.html
 */

using namespace cv;
using namespace std;

/**Function Headers*/
vector<Point2f>featureExtraction(Mat img);

int main(int argc, const char** argv) {
    
    /**Problem 2.1*/
    int f = 0; //number of frames
    VideoCapture camera;
    vector<Point2f> corners;
    Mat img;
    
    //checking whether the camera works or not
    if (!camera.open(0)) {
        printf("Can't find a camera\n");
        return 1;
    };
  
    while(true) {
        camera >> img;
        if (f%300 == 0){
            corners = featureExtraction(img);
    }
    
    for(int i = 0; i < corners.size(); i++) {
        circle(img, corners[i], 3, 255, -1);
    }
        
    f++;
        
    imshow("Feature Extraction", img);
    int key = waitKey(5);
    if (key == 27 || key == 'q') break;
}

    camera.release();

    
    /**Probelm 2.2 */
    int f = 0;
    
    //checking whether the camera works or not
    if (!camera.open(0)) {
        printf("Can't find a camera\n");
        return 1;
    };
    
    
    // Create some random colors
    vector<Scalar> colors;
    RNG rng;
    for(int i = 0; i < 100; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r,g,b));
    }
    
    Mat old_frame, old_gray, current_frame, current_gray;
    vector<Point2f> p0, p1, p2;
    
    // Take first frame and find corners in it
    camera >> old_frame;
    cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);
    
    p0 = featureExtraction(old_frame);
    
    // Create a mask image for drawing purposes
    Mat mask = Mat::zeros(old_frame.size(), old_frame.type());
    
    while(true){
        Mat frame, frame_gray;
        
        if(f%2 == 0){
        capture >> frame;
        cvtColor(current_frame, current_gray, COLOR_BGR2GRAY);
            

        
        vector<uchar> status;
        vector<float> err;
        TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
            
        // calculate optical flow
        calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(15,15), 2, criteria);
        
        for(uint i = 0; i < p0.size(); i++)
        {
            // Select good points
            if(status[i] == 1) {
                p2.push_back(p1[i]);
                
                //problem 2.3
                line(mask, p1[i], p0[i], colors[i], 2);
                circle(current_frame, p1[i], 5, colors[i], -1);
            }
        }
            
        Mat img;
        add(frame, mask, img);
        imshow("Sparse Optical Frlow Estimation", img);
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
            
        // Now update the previous frame and previous points
        old_gray = frame_gray.clone();
        p0 = p2;
    }
        f++;
}

vector<Point2f> featureExtraction(Mat img){
    Mat frame;
    cvtColor(img, frame, COLOR_BGR2GRAY); // Convert to grayscale
    vector<Point2f> corners;

    goodFeaturesToTrack(frame, corners, 200, 0.01, 10,
                        Mat(), 3, false, 0.04);
    
    return corners;
}
