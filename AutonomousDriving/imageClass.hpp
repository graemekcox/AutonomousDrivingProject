//
//  imageClass.hpp
//  AutonomousDriving
//
//  Created by Jared Cox on 2019-05-29.
//  Copyright © 2019 Graeme Cox. All rights reserved.
//

#ifndef imageClass_hpp
#define imageClass_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

class ImageClass
{
public:
    cv::Mat originalImage;
    cv::Mat blurredImage;
    cv::Mat filteredImage;
    cv::Mat maskedImage;
    cv::Mat cannyImage;
    cv::Mat cdstImage;
    cv::Mat weightedImage;
    
    ImageClass(std::string pathToImage)
    {
        originalImage =  cv::imread(pathToImage);
    };
    
    // Filtering state
    // use homogenous smoothing for some dirty filtering
    void blurImage(cv::Size size)
    {
        blurredImage = originalImage.clone();
        //blur(blurredImage, cv::dst, size);
    }
    
    // Gaussian blurring with Gaussian kernel
    void filterImage(int sigma_x = 0, int sigma_y = 0, int kernel_size = 3)
    {
        GaussianBlur(originalImage, filteredImage, cv::Size(kernel_size, kernel_size), sigma_x, sigma_y);
    }
    
    // Add yellow and white mask to remove stuff we don't care about
    void maskImage()
    {
        cv::Mat gray, hsv, mask;
        cvtColor(filteredImage, gray, CV_BGR2GRAY);
        cvtColor(filteredImage, hsv, CV_BGR2HLS);
        
        assert(filteredImage.type() == CV_8UC3);
        cv::Scalar lower = cv::Scalar(20, 100, 100); //yellow
        cv::Scalar upper = cv::Scalar(30, 255, 255);
        
        cv::Mat mask_w, mask_y;
        cv::inRange(hsv, lower, upper, mask_y);
        cv::inRange(gray, 200,255, mask_w);
        
        bitwise_or(mask_w, mask_y, maskedImage);
    }
    
    void addEdgesToImage()
    {
        Canny(maskedImage, cannyImage, 100, 150);
    }
    
    void addHoughedLinesToImage(bool displayImages)
    {

        //    Mat dst, cdst;
        //
        //    Canny(src, dst, 50, 150);
        ////    Canny
        //
        //    if (DISPLAY_IMAGES){
        //        namedWindow("Canny Image");
        //        imshow("Canny Image", dst);
        //    }
        ////    printf("Convert color\n");
        //    cvtColor(dst, cdst, CV_GRAY2BGR); //convert to color
        //
        
        // 1.output of edge detector (should be grayscale)
        cv::Mat cdst;
        /*
         1. output of edge detector (should be grayscale)
         2. A vector that will store paramteres( rho, theta) of detected lines)
         3. rho: Resolution in 'rho' pixels.
         4. Resolutino of theta in radians. Use 9pi/180) to convert to degree
         5. Theshold, minimum number of intersections to detect a line
         6. srn and stn. Defaults to zero
         */
        cvtColor(cannyImage, cdst, CV_GRAY2BGR); //convert to color
        cdstImage = cdst.clone();
        
        cv::vector<cv::Vec2f> lines;
        //    vector<Vec2f> lines;
        int rho = 1;
        int thresh = 60;
        float theta = CV_PI/180;
        int min_line_len = 10;
        int max_line_gap = 250;
        //    HoughLines(edges, lines, rho, theta, thresh, min_line_len, max_line_gap );
        HoughLines(cannyImage, lines, 1, CV_PI/180, 150,0,0);
        
        printf("Line stuff\n");
        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            cv::Point pt1, pt2;
            //        if (theta <20) continue;
            //        printf("%f\n",theta * 180/CV_PI);
            
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( cdst, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
        }
        
        //    HoughLinesP(edges, lines, 1, CV_PI/180, thresh, 10, 250);
        // Draw lines on the image
        cv::vector<cv::Vec4i> linesP;
        HoughLinesP(cannyImage, linesP, 1, CV_PI/180, 10 , min_line_len,max_line_gap);
        for (size_t i=0; i<lines.size(); i++)
        {
            cv::Vec4i l = linesP[i];
            line(cdstImage, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 3, CV_AA);
        }
        if (displayImages)
        {
            cv::namedWindow("Hough lines");
            imshow("Hough lines", cdst);
            
            cv::namedWindow("Hough Probalistic1lines");
            imshow("Hough lines", cdstImage);
        }
    }
    
    void addWeightsToImage(double alpha, double beta, double gamma)
    {
        addWeighted(cdstImage, alpha, originalImage, beta, gamma, weightedImage);
    }
    
    void displayAllImages()
    {
        std::string orig = "Original Image";
        cv::namedWindow(orig, cv::WINDOW_NORMAL);
        imshow(orig, originalImage);
        
        std::string filter_name = "Gaussian blur filtered image";
        cv::namedWindow(filter_name);
        imshow(filter_name, filteredImage);
        
        std::string mask_name = "Yellow and White masked image";
        cv::namedWindow(mask_name);
        imshow(mask_name, maskedImage);
        
        std::string canny_name = "Canny Image";
        cv::namedWindow(canny_name);
        imshow(canny_name, cannyImage);
    }
    
};

#endif /* imageClass_hpp */
