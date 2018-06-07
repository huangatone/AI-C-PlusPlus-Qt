#ifndef MYCV_H
#define MYCV_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <opencv/cv.h>
#include <opencv/cvwimage.h>

#include <iostream>
#include <stdio.h>


#include <QString>
#include <QPoint>



using namespace std;
using namespace cv;

class MyCv
{
public:
	MyCv();
};


QPoint MatchingMethod( QString src_file, QString dst_file, int md = 0 );
int CompareReslut( QPoint pos, QString src_file, QString dst_file);
float classify_gray_hist(Mat &m1, Mat &m2);
float classify_gray_hist(QString src_file, QString dst_file);

float classify_aHash(QString src_file, QString dst_file);
float classify_aHash(Mat image1,Mat image2);

float hisMatch(QString src_file, QString dst_file);


int diff(QString src_file, QString dst_file);


double image_coefficient(QString src_file, QString dst_file);

QPoint Match2(QString src_file, QString dst_file,int md = 0);

int MatchPoint(QString src_file, QString dst_file);

int CallMatchingMethod( );


#endif // MYCV_H
