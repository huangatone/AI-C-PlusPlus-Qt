#ifndef TEXTAREADETECT_H
#define TEXTAREADETECT_H

#include <opencv/cv.h>
#include <opencv/cvwimage.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
//#include <opencv2/tracking.hpp>

#include <QImage>
using namespace std;
using namespace cv;
using namespace ml;

class TextAreaDetect
{
public:
	TextAreaDetect();

	Mat preprocess(Mat gray);
	vector<RotatedRect> findTextRegion(Mat img);
	void detect(Mat img);
	int work();
};

#endif // TEXTAREADETECT_H
