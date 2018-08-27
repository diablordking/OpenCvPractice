#include <stdio.h>

#include <opencv2/opencv.hpp>

int main()
{
    cv::Mat img = cv::imread( "images/lena.png", CV_LOAD_IMAGE_COLOR ); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)
    //cv::imshow( "LENA", img );

    cv::Mat gray_img; // declare variable to hold grayscale version of img variable, gray levels wil be represented using 8 bits (uchar)
    cv::Mat gray_32f_img; // declare variable to hold grayscale version of img variable, gray levels wil be represented using 32 bits (float)

    cv::cvtColor(img, gray_img, CV_BGR2GRAY); // convert input color image to grayscale one, CV_BGR2GRAY specifies direction of conversion
    gray_img.convertTo(gray_32f_img, CV_32FC1, 1.0/255.0); // convert grayscale image from 8 bits to 32 bits, resulting values will be in the interval 0.0 - 1.0

    int x = 10, y = 15; // pixel coordinates

    float asd,asd1;
    uchar p1 = gray_img.at<uchar>(y, x); // read grayscale value of a pixel, image represented using 8 bits
    float p2 = gray_32f_img.at<float>(y, x); // read grayscale value of a pixel, image represented using 32 bits
    cv::Vec3b p3 = img.at<cv::Vec3b>(y, x); // read color value of a pixel, image represented using 8 bits per color channel

    // print values of pixels
    printf("p1 = %d\n", p1);
    printf("p2 = %f\n", p2);
    printf("p3[ 0 ] = %d, p3[ 1 ] = %d, p3[ 2 ] = %d\n", p3[ 0 ], p3[ 1 ], p3[ 2 ]);

    gray_img.at<uchar>(y, x) = 0; // set pixel value to 0 (black)

    // draw a rectangle
    cv::rectangle(gray_img, cv::Point(65, 84), cv::Point(75, 94),
                  cv::Scalar(50), CV_FILLED);

    // declare variable to hold gradient image with dimensions: width= 256 pixels, height= 50 pixels.
    // Gray levels wil be represented using 8 bits (uchar)
    cv::Mat gradient_8uc1_img(50, 256, CV_8UC1);
    int gamma = 2;
    // For every pixel in image, assign a brightness value according to the x coordinate.
    // This wil create a horizontal gradient.
    for (int y = 0; y < gray_32f_img.rows; y++) {
        for (int x = 0; x < gray_32f_img.cols; x++) {
                asd =gray_32f_img.at<float>(y, x);
        asd1 = std::pow (asd,gamma);
        gray_32f_img.at<float>(y, x) = asd1;
          }
    }

    // diplay images
    cv::imshow("Gradient", gradient_8uc1_img);
    cv::imshow( "Lena gray", gray_img);
    cv::imshow( "Lena gray 32f", gray_32f_img);

    cv::waitKey( 0 ); // wait until keypressed

    return 0;
}
