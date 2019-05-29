//
//  main.cpp
//  AutonomousDriving
//
//  Created by Graeme Cox on 2019-05-26.
//  Copyright © 2019 Graeme Cox. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void displayOriginal(Mat image);
void displayBrightness(Mat image);
Mat mask_yw(Mat image);
Mat getHoughLines(Mat src);


int main(int argc, const char * argv[]) {
    
    Mat image = imread("/Users/graemecox/Documents/Programming/Projects/AutonomousDriving/AutonomousDriving/solidYellowLeft.jpg");
    
    if (image.empty()){
        cout << "Could not open or find the image" << endl;
        cin.get();
        return -1;
        
    }
    
    
    // Add yellow and white mask to remove stuff we don't care about
    Mat mask;
    mask = mask_yw(image);


    Mat cdst = getHoughLines(mask);
    displayOriginal(cdst);
    return 0;
}

Mat mask_yw(Mat image)
{
    Mat gray, hsv, mask;
    cvtColor(image, gray, CV_BGR2GRAY);
    cvtColor(image, hsv, CV_BGR2HLS);
    
    assert(image.type() == CV_8UC3);
    cv::Scalar lower = cv::Scalar(20, 100, 100); //yellow
    cv::Scalar upper = cv::Scalar(30, 255, 255);

    Mat mask_w, mask_y;
    cv::inRange(hsv, lower, upper, mask_y);
    cv::inRange(gray, 200,255, mask_w);
    
    bitwise_or(mask_w, mask_y, mask);
//    displayOriginal(mask_w);
//    displayOriginal(mask_y);
    return mask;
    
}

void displayOriginal(Mat image) {
    String windowname = "Original";
    
    namedWindow(windowname, WINDOW_NORMAL);
    imshow(windowname, image);
    waitKey(0);
    destroyWindow(windowname);
}

void displayBrightness(Mat image) {
    String windowname = " The Car";
    
    // Testing brightness
    Mat imageBrightHigh50;
    image.convertTo(imageBrightHigh50, -1, 1, 50);
    
    Mat imageBrightHigh100;
    image.convertTo(imageBrightHigh100,-1, 1, 100);
    
    Mat imageBrightLow50;
    image.convertTo(imageBrightLow50, -1, 1, -50);
    
    Mat imageBrightLow100;
    image.convertTo(imageBrightLow100, -1, 1, -100);
    
    String origName = "Original Car";
    String nameBright50 = "Bright 50 Car";
    String nameBright100 = "Bright 100 Car";
    String nameLessBright50 = "Bright -50 Car";
    String nameLessBright100 = "Bright -100 Car";
    
    namedWindow(origName, WINDOW_NORMAL);
    namedWindow(nameBright50, WINDOW_NORMAL);
    namedWindow(nameBright100, WINDOW_NORMAL);
    namedWindow(nameLessBright50, WINDOW_NORMAL);
    namedWindow(nameLessBright100, WINDOW_NORMAL);
    
    imshow(origName, image);
    imshow(nameBright50, imageBrightHigh50);
    imshow(nameBright100, imageBrightHigh100);
    imshow(nameLessBright50, imageBrightLow50);
    imshow(nameLessBright100, imageBrightHigh100);
    
    waitKey(0);
    destroyWindow(windowname);
    
}


Mat getHoughLines(Mat src)
{
    Mat dst, cdst;
    Canny(src, dst, 50, 150, 3);
    
    printf("Convert color\n");
    cvtColor(dst, cdst, CV_GRAY2BGR);
    
    vector<Vec2f> lines;
    printf("Hough lines\n");
    // 1.output of edge detector (should be grayscale)
    
    /*
     1. output of edge detector (should be grayscale)
     2. A vector that will store paramteres( rho, theta) of detected lines)
     3. rho: Resolution in 'rho' pixels.
     4. Resolutino of theta in radians. Use 9pi/180) to convert to degree
     5. Theshold, minimum number of intersections to detect a line
     6. srn and stn. Defaults to zero
     */
    int rho = 2;
    int thesh = 40;
    float theta = CV_PI/180;
    int min_line_len = 50;
    int max_line_gap = 200;
    HoughLines(dst, lines, rho, theta, thesh, min_line_len, max_line_gap );
    
    printf("Line stuff\n");
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }
    
    return cdst;

}
