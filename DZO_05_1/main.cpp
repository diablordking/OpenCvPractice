#include<opencv2/opencv.hpp>

#include <math.h>
#include<iostream>

int main(int argc, char* argv[])
{
    cv::Mat img;
    img = cv::imread("images/lena64.png", CV_LOAD_IMAGE_COLOR); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)
    //todo: read in gray scale, then conver it

    cv::Mat gray_img; // declare variable to hold grayscale version of img variable, gray levels wil be represented using 8 bits (uchar)

    cv::cvtColor(img, gray_img, CV_BGR2GRAY);

    cv::Mat gray_64fc1_img;
    cv::Mat gray_64fc1_img_powerSpectrum;
    cv::Mat img_phase;

    gray_img.convertTo(gray_64fc1_img, CV_64FC2, 1.0 / 255.0);
    gray_img.convertTo(gray_64fc1_img_powerSpectrum, CV_64FC2, 1.0 / 255.0);
    img_phase= gray_64fc1_img_powerSpectrum.clone();

    cv::imshow("earth", img);

    double realPartFreq = 0;
    double imgPartFreq = 0;
    double powerSpectrum = 0;
    double phase = 0;

    int M = gray_img.rows;
    int N = gray_img.cols;

    cv::Mat matrixStoreValues = cv::Mat::zeros(M, N, CV_64FC2);

    for (int k = 0; k < gray_img.rows; k++)
    {
        for (int l = 0; l < gray_img.cols; l++)
        {
            realPartFreq = 0;
            imgPartFreq = 0;
            powerSpectrum = 0;

            for (int m = 0; m < gray_img.rows; m++)
            {
                for (int n = 0; n < gray_img.cols; n++)
                {
                    double brightness = gray_64fc1_img.at<double>(m, n);
                    double x = -2 * CV_PI * (m*(double)k / (double)M + n*(double)l / (double)N);
                    double realPartFreqBuf = brightness * 1 / (sqrt(M*N)) * cos(x);
                    double  imgPartFreqBuf = brightness * 1 / (sqrt(M*N)) * sin(x);
                    realPartFreq += realPartFreqBuf;
                    imgPartFreq += imgPartFreqBuf;
                }
            }

            matrixStoreValues.at<cv::Vec2d>(k, l)[0] = realPartFreq;
            matrixStoreValues.at<cv::Vec2d>(k, l)[1] = imgPartFreq;

            phase = atan(imgPartFreq / realPartFreq);

            powerSpectrum = sqrt( pow(realPartFreq, 2) + pow(imgPartFreq, 2));
            powerSpectrum = log(powerSpectrum * powerSpectrum);

            gray_64fc1_img_powerSpectrum.at<double>(k, l) = powerSpectrum;
            img_phase.at<double>(k, l) = phase;
        }
    }

    cv::Mat img_spectrum_new = gray_64fc1_img_powerSpectrum.clone();
    cv::normalize(gray_64fc1_img_powerSpectrum, img_spectrum_new, 0, 1, cv::NORM_MINMAX);

	cv::imshow("result", gray_64fc1_img_powerSpectrum);

    cv::imshow("spectrumNew", img_spectrum_new);



    cv::imshow("phase", img_phase);


    ////////////////////  exercise 6  /////////////////////////////
    cv::Mat invertImage = cv::Mat::zeros(M, N, CV_64FC1);
    //cv::Mat invertImage = gray_64fc1_img.clone();
    for (int m = 0; m < M; m++)
    {
        for (int n = 0; n < N; n++)
        {
            double sumVal = 0;

            for (int k = 0; k < M; k++)
            {
                for (int l = 0; l < N; l++)
                {
                    double x = 2 * CV_PI * (m*(double)k/M + n*(double)l/N);
                    double F_real = matrixStoreValues.at<cv::Vec2d>(k, l)[0];
                    double F_img = matrixStoreValues.at<cv::Vec2d>(k, l)[1];
                    double tempVal = F_real*cos(x) - F_img*sin(x) ;
                    sumVal += tempVal;
                }
            }

            invertImage.at<double>(m, n) = 1 / sqrt(M*N) * sumVal;
        }
    }

    cv::imshow("invertImage", invertImage);


    cv::waitKey(0); // wait until keypressed

    return 0;
}
