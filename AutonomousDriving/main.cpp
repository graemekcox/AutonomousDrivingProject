//
//  main.cpp
//  AutonomousDriving
//
//  Created by Graeme Cox on 2019-05-26.
//  Copyright © 2019 Graeme Cox. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "imageClass.hpp"

using namespace std;
using namespace cv;

#define DISPLAY_IMAGES 0
#define IMAGE_PATH "/Users/jaredcox/Documents/Programming/Projects/AutonomousDrivingProject/AutonomousDriving/Resources/Images/solidYellowLeft.jpg"

void displayOriginal(Mat image);
void displayBrightness(Mat image);
Mat mask_yw(Mat image);
Mat getHoughLines(Mat src);

int main(int argc, const char * argv[])
{
    ImageClass* image = new ImageClass("/Users/jaredcox/Documents/Programming/Projects/AutonomousDrivingProject/AutonomousDriving/Resources/Images/solidYellowLeft.jpg");
    
    if (image->originalImage.empty())
    {
        cout << "Could not open or find the image" << endl;
        cin.get();
        
        return -1;
    }
    
    // Filtering state
    // use homogenous smoothing for some dirty filtering, NOT IMPLEMENTED
    image->blurImage(Size(5,5));
    
    // Gaussian blurring with Gaussian kernel: sigma x, sigma y, kernel size
    image->filterImage(0, 0, 3);
 
    // Add yellow and white mask to remove stuff we don't care about
    image->maskImage();

    //FIXME Should do region of interest here
    // Region of interest
//    int rows = image.rows;
//    int cols = image.cols;
//
//    int start_x = cols/5, start_y = rows/3;
//    printf("Start = %d %d\n", start_x, start_y);
//
//    Rect roi(start_x,0, cols * 3/5,rows);
//
//    printf("%d %d\n",rows, cols);
//
//    Mat image_roi = image(roi);
//    displayOriginal(image_roi);
//
//    Mat roi_mask = Mat(rows, cols, image.type());
    
    image->addEdgesToImage();
    
    //Display different stages.
    if (DISPLAY_IMAGES)
    {
        image->displayAllImages();
    }
    //    printf("Convert color\n");
//    cvtColor(dst, cdst, CV_GRAY2BGR); //convert to color
//
//    Mat contours;
////    _, contours, hierarchy = cv::findContours(edges, RETR_TREE, CHAIN_APPROX_NONE);
//    findContours(edges, contours, RETR_TREE, CHAIN_APPROX_NONE);
//

    image->addHoughedLinesToImage(DISPLAY_IMAGES);
    
    //Add Weights: alpha, beta, gamma.
    image->addWeightsToImage(0.8, 1.0, 0.0);
    
    String weight_name = "Final Weighted image";
    namedWindow(weight_name);
    imshow(weight_name, image->cdstImage);
    
//    if (DISPLAY_IMAGES)
    waitKey(0);
    
    return 0;
}

void displayOriginal(Mat image)
{
    String windowname = "Original";
    
    namedWindow(windowname, WINDOW_NORMAL);
    imshow(windowname, image);
    waitKey(0);
    destroyWindow(windowname);
}


