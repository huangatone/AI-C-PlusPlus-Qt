#ifndef CVFUNCTION_H
#define CVFUNCTION_H
#include <opencv/cv.h>
#include <opencv/cvwimage.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <QImage>
using namespace std;
using namespace cv;

//MYMEMO:  Call back function
class CVFunction
{
public:
	CVFunction();

	void salt(Mat &image, int n);
	void colorReduce0(cv::Mat &image, int div);
	void sharpen(const cv::Mat &image, cv::Mat &result) ;
	void sharpen2D(const cv::Mat &image, cv::Mat &result);

	QImage Mat2QImage(const cv::Mat &mat);
	QImage Mat2QImage1(const cv::Mat &mat);

	Mat HistogramRGB(QString file_name);

	Mat Histogram1D(QString file_name);

	void getText(Mat image);

	cv::Mat getHistogram(const cv::Mat &image);
	cv::Mat getHistogramImage(const cv::Mat &image);


	void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);
	int check();

	cv::Mat face_detect(Mat frame);
	cv::Mat sharp_detect(Mat &img);
	Mat text_detect( Mat image);
	int text_detect2( Mat image);

	void setCallbackFunc(void (*func) (QPointF point))
	{
		callbackFunc = func;
		callbackFunc(QPointF(0,0));
	}

	int polygonfind( void );

	int polygonfind(  Mat image );

	void find();

private:

	void (*callbackFunc)(QPointF point);

	cv::Mat rgb;


	int histSize[1];
	float hranges[2];
	const float* ranges[1];
	int channels[1];

	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;
};

#endif // CVFUNCTION_H
