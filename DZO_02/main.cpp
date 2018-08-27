#include <stdio.h>

#include <opencv2/opencv.hpp>

#include <iostream>


using namespace std;
using namespace cv;


void noBorderProcessing(Mat src, Mat dst, float Kernel[3][3])
{

    float sum;
    for(int y = 1; y < src.rows - 1; y++){
        for(int x = 1; x < src.cols - 1; x++){
            sum = 0.0;
            for(int k = -1; k <= 1;k++){
                for(int j = -1; j <=1; j++){
                    sum = sum + Kernel[j+1][k+1]*src.at<uchar>(y - j, x - k);
                }
            }
            dst.at<uchar>(y,x) = sum;
        }
    }
}
int main()
{
    cv::Mat img = cv::imread( "images/lena.png", CV_LOAD_IMAGE_COLOR ); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)
    cv::Mat gray_img;
    cv::Mat gray_32f_img;
    cv::cvtColor(img, gray_img, CV_BGR2GRAY);
    gray_img.convertTo(gray_32f_img, CV_32FC1, 1.0/255.0);
    Mat new_image = gray_img.clone();



    float Kernel[3][3] = {
                            {1/16.0, 1/8.0, 1/16.0},
                            {1/8.0, 1/4.0, 1/8.0},
                            {1/16.0, 1/8.0, 1/16.0}
                           };

     /// Declare variables

    for(int y = 1; y < img.rows - 1; y++){
        for(int x = 1; x < img.cols - 1; x++){
            float sum = 0.0;
            for(int k = -1; k <= 1;k++){
                for(int j = -1; j <=1; j++){
                    sum = sum + Kernel[j+1][k+1]*gray_img.at<uchar>(y - j, x - k);
                }
            }
            new_image.at<uchar>(y,x) = sum;
        }
    }


    cv::imshow( "Lena", img);
    cv::imshow("New Image", new_image);


    cv::waitKey( 0 ); // wait until keypressed

    return 0;
}
