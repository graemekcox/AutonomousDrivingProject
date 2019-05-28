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

int main(int argc, const char * argv[]) {
    
    Mat image = imread("/Users/graemecox/Documents/Programming/Projects/AutonomousDriving/AutonomousDriving/solidYellowLeft.jpg");
    
    if (image.empty()){
        cout << "Could not open or find the image" << endl;
        cin.get();
        return -1;
        
    }

    Mat dst, cdst;
    Canny(image, dst, 50, 200, 3);
    printf("Convert color\n");
    cvtColor(dst, cdst, CV_GRAY2BGR);
    

    vector<Vec2f> lines;
    printf("Hough lines\n");
    HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );
    
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
//    imshow("Source", image);
//    imshow("Detected Lines", cdst);
    displayOriginal(cdst);
//    Mat gray;
//    cvtColor(image, gray, COLOR_BGR2GRAY);
////    Mat gray = cvtColor(image, COLOR_BGR2GRAY);
//    Mat edges; // for all edges
//
//    Canny(gray, edges, 50, 200);
//    vector<Vec4i> lines;
//
//    HoughLines(edges, lines, 1, CV_PI/180, 100, 0, 0);
////    HoughLines(edges, lines, 1, CV_PI/180, 20, 10, 250);
//
//    for (size_t i=0; i<lines.size(); i++) {
////        Vec4i l = lines[i];
////        line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scaler(255,0,0), 3, LINE_AA);
//        float rho = lines[i][0], theta = lines[i][1];
//        Point pt1, pt2;
//        double a = cos(theta), b = sin(theta);
//        double x0 = a*rho, y0 = b*rho;
//        pt1.x = cvRound(x0 + 1000*(-b));
//        pt1.y = cvRound(y0 + 1000*(a));
//        pt2.x = cvRound(x0 - 1000*(-b));
//        pt2.y = cvRound(y0 - 1000*(a));
//        line( edges, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
//    }
    
    return 0;
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
