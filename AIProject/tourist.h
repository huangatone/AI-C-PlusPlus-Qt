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

	//static void onCandyTrackbar(int, void* p);
	//int doCanny();


protected:
	// 声明 原始图片，灰度图片，和 canny边缘图片
	Mat image, cedge;
	Mat gray, edge;

};

#endif // TOURIST_H
