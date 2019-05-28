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

void displayBrightness(Mat image);

int main(int argc, const char * argv[]) {
    
    Mat image = imread("/Users/graemecox/Documents/Programming/Projects/AutonomousDriving/AutonomousDriving/car.jpg");
    
    if (image.empty()){
        cout << "Could not open or find the image" << endl;
        cin.get();
        return -1;
        
    }
    
    displayBrightness(image);
    
    return 0;
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
