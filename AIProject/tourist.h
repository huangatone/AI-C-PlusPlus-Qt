#ifndef TOURIST_H
#define TOURIST_H

#include <opencv/cv.h>
#include <opencv/cvwimage.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <QImage>
using namespace std;
using namespace cv;



class Tourist
{
public:
	Tourist();

	Mat onCandy(QString f_n,int iThresh=1);
	Mat onChangeThresh(int iThresh);

	Mat random_kmeans( );

	Mat segmentation();

	Mat Seamless_Clone(QString src, QString dst, QString m="");
	Mat Seamless_Clone(QString src, QString dst, int n);

	Mat  addImage(QString f1, QString f2, double a, double b );

	Mat onBlue(QString src, int len);
	Mat onGaussianBlur(QString src,int len);
	Mat onMedianBlur(QString src,int len);
	Mat onBilateralFilter(QString src_file,int len);

	//static void onCandyTrackbar(int, void* p);
	//int doCanny();

	//Advanced Edge Detection
	Mat Advanced_Edge_Detection(QString src,int sz, int nThresh1, int nThresh2 );


protected:
	// 声明 原始图片，灰度图片，和 canny边缘图片
	Mat image, cedge;
	Mat gray, edge;

};

#endif // TOURIST_H
