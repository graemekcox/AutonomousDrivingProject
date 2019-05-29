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
    
    //Add Region of Interest:  IN PROGRESS!
    image->addRegionOfInterest();

    //Canny image
    image->addEdgesToImage();
    
    //Display different stages.
    if (DISPLAY_IMAGES)
    {
        image->displayAllImages();
    }
    
    //IN PROGRESS!
    image->addContoursToImage();

    image->addHoughedLinesToImage(DISPLAY_IMAGES);
    
    //Add Weights: alpha, beta, gamma.
    image->addWeightsToImage(0.8, 1.0, 0.0);
    
    String weight_name = "Final Weighted image";
    namedWindow(weight_name);
    imshow(weight_name, image->cdstImage);
    
    waitKey(0);
    
    return 0;
}
