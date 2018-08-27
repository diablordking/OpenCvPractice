#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <cmath>
#include <math.h>       /* atan */
#include <iostream>
#define SQR( x ) ( ( x ) * ( x ) )
#define DEG2RAD( x ) ( ( x ) * M_PI / 180.0 )
#define MY_MIN( X, Y ) ( ( X ) < ( Y ) ? ( X ) : ( Y ) )
#define PI 3.14159265

using namespace cv;
using namespace std;

struct RLDUserData {
    cv::Mat & src_8uc3_img;
    cv::Mat & undistorted_8uc3_img;
    int K1;
    int K2;

    RLDUserData( const int K1, const int K2, cv::Mat & src_8uc3_img, cv::Mat & undistorted_8uc3_img ) : K1( K1 ), K2( K2 ), src_8uc3_img( src_8uc3_img ), undistorted_8uc3_img( undistorted_8uc3_img ) {

    }
};


void geom_dist( cv::Mat & src_8uc3_img, cv::Mat & dst_8uc3_img, bool bili, double K1 = 1.0, double K2 = 1.0 )
{
    float halfWidth = src_8uc3_img.cols / 2;
    float halfHeight = src_8uc3_img.rows / 2;
    int theta;
    float strength = K2;
    int correctionRadius = sqrt(pow(src_8uc3_img.cols,2)+ pow(src_8uc3_img.rows,2)) / strength;
    float zoom = 1;

    for(int y = 1; y < dst_8uc3_img.rows; y++)
    {
        for(int x = 1; x < dst_8uc3_img.cols; x++)
        {
            float bigr = sqrt(pow(halfHeight,2)+pow(halfWidth,2));
            float oldX = ((x - halfWidth)/bigr);
            float oldY = ((y - halfHeight)/bigr);
            float r2 = ((pow(oldX,2))+pow(oldY,2));

            float yuvarlak = 1+K1*r2+K2*(pow(r2,2));
            float newX = (x-halfWidth)*(1/yuvarlak)+halfWidth;
            float newY = ((y-halfHeight)*(1/yuvarlak))+halfHeight;


          //  cout << "Source X=" <<newX  << endl;
         //   cout << "Source Y=" <<newY << endl;
            dst_8uc3_img.at<cv::Vec3b>(y,x) = src_8uc3_img.at<cv::Vec3b>(newY,newX);
        }
    }
}



void apply_rld( int id, void * user_data )
{
    RLDUserData *rld_user_data = (RLDUserData*)user_data;

    geom_dist( rld_user_data->src_8uc3_img, rld_user_data->undistorted_8uc3_img, !false, rld_user_data->K1 / 100.0, rld_user_data->K2 / 100.0 );
    cv::imshow( "Geom Dist", rld_user_data->undistorted_8uc3_img );
}

int main()
{
    cv::Mat src_8uc3_img, geom_8uc3_img;

    src_8uc3_img = cv::imread( "images/distorted_window.jpg", CV_LOAD_IMAGE_COLOR );
    if ( src_8uc3_img.empty() )
    {
        printf( "Unable to load image!\n" );
        exit( -1 );
    }

    cv::namedWindow( "Original Image" );
    cv::imshow( "Original Image", src_8uc3_img );


    geom_8uc3_img = src_8uc3_img.clone();
    RLDUserData rld_user_data( 3.0, 1.0, src_8uc3_img, geom_8uc3_img );

    apply_rld( 0, (void*)&rld_user_data );

    cv::namedWindow( "Geom Dist" );
    cv::imshow( "Geom Dist", geom_8uc3_img );

    cv::createTrackbar( "K1", "Geom Dist", &rld_user_data.K1, 100, apply_rld, &rld_user_data );
    cv::createTrackbar( "K2", "Geom Dist", &rld_user_data.K2, 100, apply_rld, &rld_user_data );

    cv::waitKey( 0 );

    return 0;
}
