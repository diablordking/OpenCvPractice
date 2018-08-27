#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <stdint.h>


using namespace cv;
using namespace std;

float sigma = 0.015;
float lambda = 0.1;

double g (float dI) {
    return exp ( - pow (dI, 2) / pow (sigma, 2) );
}

double e (double x) {
    return cos(x);
}

double ie (double x) {
    return sin(x);
}

int main () {
    cv::Mat src_8uc1_img = cv::imread ("images/lena64.png", CV_LOAD_IMAGE_GRAYSCALE);

    cv::Mat gray_32ft1_img;
    src_8uc1_img.convertTo(gray_32ft1_img, CV_32FC1, 1.0/255.0);

    int M = gray_32ft1_img.rows;
    int N = gray_32ft1_img.cols;

    gray_32ft1_img.convertTo(gray_32ft1_img, CV_32FC1, 1.0 / sqrt(M * N));

    cv::Mat res_phase = cv::Mat::zeros (gray_32ft1_img.size(),
                                    gray_32ft1_img.type() );

    cv::Mat res_power = cv::Mat::zeros (gray_32ft1_img.size(),
                                    gray_32ft1_img.type() );

    cv::Mat F = cv::Mat::zeros(gray_32ft1_img.size(),
                               CV_32FC2 );

    float x, phi_re, phi_im, f, amplitude, phase, P, R, I, logP;

    cv::imshow ("Source", gray_32ft1_img);
    for (int k = 0; k < M; k++) {
        for (int l = 0; l < N; l++) {
            R = 0;
            I = 0;
            for (int m = 0; m < M; m++) {
                for (int n = 0; n < N; n++) {
                    x = 2 * M_PI * (((m * k) / (float)M) + ((n * l) / (float)N));

                    phi_re = e(x);
                    phi_im = -ie(x);

                    f = gray_32ft1_img.at<float> (m, n);

                    R += f * phi_re;
                    I += f * phi_im;
                }


            }

            F.at<cv::Vec2f>(k, l) = cv::Vec2f(R, I);

            amplitude = sqrt(R*R + I*I);
            phase = atan(I / R);

            P = amplitude*amplitude;
            logP = log(P);

            res_phase.at<float> (k, l) = R+I;
            res_power.at<float> (k, l) = P;
        }
    }
    res_phase = res_phase(Rect(0, 0, res_phase.cols & -2, res_phase.rows & -2));
    int cx = res_phase.cols/2;
    int cy = res_phase.rows/2;

    Mat q0(res_phase, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(res_phase, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(res_phase, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(res_phase, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);





    //cv::Mat normalized = gray_32ft1_img.clone();

    cv::imshow("Phase", res_phase);
    cv::imshow("Power", src_8uc1_img);

    cv::waitKey( 0 ); // wait until keypressed
    return 0;
}
