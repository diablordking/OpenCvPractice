#include <stdio.h>
#include <stdint.h>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;


void computeDft(const vector<double> &inreal, const vector<double> &inimag,
                vector<double> &outreal, vector<double> &outimag)
{

    size_t n = inreal.size();
    for (size_t k = 0; k < n; k++)    // For each output element
    {
        double sumreal = 0;
        double sumimag = 0;
        for (size_t t = 0; t < n; t++)    // For each input element
        {
            double angle = 2 * M_PI * t * k / n;
            sumreal +=  inreal[t] * cos(angle) + inimag[t] * sin(angle);
            sumimag += -inreal[t] * sin(angle) + inimag[t] * cos(angle);
        }
        outreal[k] = sumreal;
        outimag[k] = sumimag;
    }
}

void takeDFT(Mat& inreal, Mat& inimag,Mat &outreal, Mat &outimag )
{
    double n = inreal.rows * inreal.cols;
    for (int k = 0; k < n; k++)
    {
        double sumreal = 0;
        double sumimag = 0;
        for (int t = 0; t < n; t++)    // For each input element
        {
            double angle = 2 * M_PI * t * k / n;
            sumreal +=  inreal.at<double>(t,k) * cos(angle) + inimag.at<double>(t,k) * sin(angle);
            sumimag += -inreal.at<double>(t,k) * sin(angle) + inimag.at<double>(t,k) * cos(angle);

        }

    }



}


void showDFT(Mat& source)
{

    Mat splitArray[2] = {Mat::zeros(source.size(),CV_32F),Mat::zeros(source.size(),CV_32F)};

    split(source,splitArray);

    Mat dftMagnitude;

    magnitude(splitArray[0],splitArray[1],dftMagnitude);

    dftMagnitude += Scalar::all(1);

    log(dftMagnitude,dftMagnitude);

    normalize (dftMagnitude,dftMagnitude,0,1,CV_MINMAX);

    imshow("DFT",dftMagnitude);

    waitKey();

}
int main()
{
    Mat original = imread( "images/lena.png", CV_LOAD_IMAGE_GRAYSCALE ); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)
    //cv::imshow( "LENA", img );
    Mat originalFloat;


    original.convertTo(originalFloat ,CV_32FC1, 1.0 / 255.0);
    Mat imaganiry = Mat::zeros(originalFloat.size(),CV_32FC1);
    Mat outreal = Mat::zeros(originalFloat.size(),CV_32FC1);
    Mat outimage = Mat::zeros(originalFloat.size(),CV_32FC1);

    takeDFT(originalFloat, imaganiry,outreal,outimage);

    magnitude(outreal, outimage, outreal);// planes[0] = magnitude
    Mat magI = outreal;
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).

    imshow("Input Image"       , original  );    // Show the result
    imshow("spectrum magnitude", magI);
    waitKey();


    return 0;
}
