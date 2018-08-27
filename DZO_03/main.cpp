#include <stdio.h>
#include <math.h>       /* exp */

#include <opencv2/opencv.hpp>

int main()
{
    cv::Mat img = cv::imread( "images/valve.png", CV_LOAD_IMAGE_GRAYSCALE ); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)
    //cv::imshow( "LENA", img );
    cv::Mat gray_img;
    img.convertTo( gray_img, CV_64FC1, 1.0 / 255.0 );
    cv::Mat imagary_img=gray_img.clone();
    // This wil create a horizontal gradient.
    double sigma = 0.015;
    double Lamda = 0.1;



    for(int k = 0; k <= 50; k++)
    {
        for(int y = 1; y < gray_img.rows - 1; y++)
        {
            for(int x = 1; x < gray_img.cols - 1; x++)
            {
                double center = gray_img.at<double>( y, x );

                double deltaN = fabs(gray_img.at<double>(y-1, x) - center );

                double ust = -((pow(deltaN,2))/(pow(sigma,2)));

                double Cn = exp (ust);



                double deltaS = fabs(gray_img.at<double>(y+1, x+1)- center);

                double ust1 = -((pow(deltaS,2))/(pow(sigma,2)));

                double Cs = exp (ust1);



                double deltaE = fabs(gray_img.at<double>(y, x+1)-center);

                double ust2 = -((pow(deltaE,2))/(pow(sigma,2)));

                double Ce = exp (ust2);



                double deltaW = fabs(gray_img.at<double>(y-1, x)- center);

                double ust3 = -((pow(deltaW,2))/(pow(sigma,2)));

                double Cw = exp (ust3);


                imagary_img.at<double>(y, x) = (gray_img.at<double>(y, x)*(1.0-Lamda*(Cn+Cs+Ce+Cw)))+Lamda*(Cn*gray_img.at<double>(y, x -1)+Cs*gray_img.at<double>(y, x+1)+Ce*gray_img.at<double>(y+1, x)+gray_img.at<double>(y-1, x)*Cw);

            }
        }
        for ( int y = 0; y < imagary_img.rows; y++ ) {
            for ( int x = 0; x < imagary_img.cols; x++ ) {
                gray_img.at<double>( y, x ) = imagary_img.at<double>( y, x );
            }
        }
    }

    cv::Mat new_img=imagary_img.clone();

    cv::imshow( "Original", img);
    cv::imshow( "Filter", imagary_img);

    cv::waitKey( 0 ); // wait until keypressed

    return 0;
}
