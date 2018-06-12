#include <stdio.h>
#include <iostream>
#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O


using namespace cv;
using namespace std;

int main()
{
 
 //read 2 images for histogram comparing
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 Mat imgA, imgB;
 imgA = imread("/rong/ss1.png");
 imgB = imread("/rong/ss2.png");

 
 imshow("img1", imgA);
 imshow("img2", imgB);


 //variables preparing
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int hbins = 30, sbins = 32; 
 int channels[] = {0,  1};
 int histSize[] = {hbins, sbins};
 float hranges[] = { 0, 180 };
 float sranges[] = { 0, 255 };
 const float* ranges[] = { hranges, sranges}; 

 Mat patch_HSV;
 MatND HistA, HistB;

 //cal histogram & normalization
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 cvtColor(imgA, patch_HSV, CV_BGR2HSV);
 calcHist( &patch_HSV, 1, channels,  Mat(), // do not use mask
     HistA, 2, histSize, ranges,
     true, // the histogram is uniform
     false );
 normalize(HistA, HistA,  0, 255, CV_MINMAX);


 cvtColor(imgB, patch_HSV, CV_BGR2HSV);
 calcHist( &patch_HSV, 1, channels,  Mat(),// do not use mask
     HistB, 2, histSize, ranges,
     true, // the histogram is uniform
     false );
 normalize(HistB, HistB, 0, 255, CV_MINMAX);
 
 
 
 
 //compare histogram
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //비교하기
 float bc = compareHist(HistA, HistB, CV_COMP_BHATTACHARYYA); 
 printf("(The range of matcing value is 0~1, 0 is best matching, 1 means miss matching\n");
 printf("V = %lf \n", bc);


 waitKey(0);

 return 0;
}