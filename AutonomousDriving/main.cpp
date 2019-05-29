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


#define DISPLAY_IMAGES 0

void displayOriginal(Mat image);
void displayBrightness(Mat image);
Mat mask_yw(Mat image);
Mat getHoughLines(Mat src);


int main(int argc, const char * argv[]) {
    
    Mat image = imread("/Users/graemecox/Documents/Programming/Projects/AutonomousDriving/AutonomousDriving/solidWhiteRight.jpg");
    
    if (image.empty()){
        cout << "Could not open or find the image" << endl;
        cin.get();
        return -1;
        
    }
    Mat filtered;

    // Filtering state
    // use homogenous smoothing for some dirty filtering
//    blur(image, dst, Size(5,5));
    
    // Gaussian blurring with Gaussian kerne
    int sigma_x = 0, sigma_y = 0;
    GaussianBlur(image, filtered, Size(5,5), sigma_x, sigma_y);
    
 
    // Add yellow and white mask to remove stuff we don't care about
    Mat mask, cdst;
    mask = mask_yw(filtered);

    // Displaying different stages
    if (DISPLAY_IMAGES){
    String orig = "Original Image";
    namedWindow(orig, WINDOW_NORMAL);
    imshow(orig, image);
    
    String filter_name = "Gaussian blur filtered image";
    namedWindow(filter_name);
    imshow(filter_name, filtered);
    
    String mask_name = "Yellow and White masked image";
    namedWindow(mask_name);
    imshow(mask_name, mask);
    }
    //FIXME Should do region of interest here
    
    
    cdst = getHoughLines(mask);
//    displayOriginal(cdst);
    double alpha = 0.8, beta=1.0, gamma = 0.0;
    
    //Weighted image
    Mat weight_img;
    addWeighted(cdst, alpha, image, beta, gamma, weight_img);
    
    String weight_name = "Final Weighted image";
    namedWindow(weight_name);
    imshow(weight_name, weight_img);
    
//    if (DISPLAY_IMAGES)
    waitKey(0);
    
    
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
    
    return mask;
    
}

void displayOriginal(Mat image) {
    String windowname = "Original";
    
    namedWindow(windowname, WINDOW_NORMAL);
    imshow(windowname, image);
    waitKey(0);
    destroyWindow(windowname);
}

Mat getHoughLines(Mat src)
{
    Mat dst, cdst;
    
    Canny(src, dst, 50, 150);
//    Canny
    
    if (DISPLAY_IMAGES){
        namedWindow("Canny Image");
        imshow("Canny Image", dst);
    }
//    printf("Convert color\n");
    cvtColor(dst, cdst, CV_GRAY2BGR); //convert to color
    

    // 1.output of edge detector (should be grayscale)
    
    /*
     1. output of edge detector (should be grayscale)
     2. A vector that will store paramteres( rho, theta) of detected lines)
     3. rho: Resolution in 'rho' pixels.
     4. Resolutino of theta in radians. Use 9pi/180) to convert to degree
     5. Theshold, minimum number of intersections to detect a line
     6. srn and stn. Defaults to zero
     */
    vector<Vec2f> lines;
    int rho = 2;
    int thesh = 60;
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
    
    if (DISPLAY_IMAGES){
        namedWindow("Hough lines");
        imshow("Hough lines", cdst);
    }
        
    return cdst;

}
