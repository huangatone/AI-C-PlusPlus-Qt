#ifndef CVFUNCTION_H
#define CVFUNCTION_H
#include <opencv/cv.h>
#include <opencv/cvwimage.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <QImage>
using namespace std;
using namespace cv;

/*
 *
 *
 * 图像加载与数据提取

图像像素访问和点操作

对比度增强

全局和局部直方图均衡化，直方图绘制

图像平滑

图像锐化

边缘检测

基于颜色的分割

颜色风格转换

煤层雕刻

Harris角点检测

特征检测与匹配

图像拼接

伽玛校正和白平衡
 */
//Image Stitching
Mat crop_black_borders(Mat img);
Mat Image_Stitching( QString file_name1, QString file_name2);

//Advanced Edge Detection
Mat Advanced_Edge_Detection(QString file_name,QString dir_name);


//Colour Segmentation
int findBiggestContour(std::vector<std::vector<Point> > contours);
Mat Colour_Segmentation(QString file_name);


//Colour Transfer
QList<Mat> Colour_Transfer(QStringList file_lt);

//----------------
Mat Contrast_Enhancement(QString file_name);

//---------
Mat Edge_Detection(QString file_name);


//Histogram Equalization
Mat Histogram_Equalization(QString f_name);

//Harris Corner Detector--
Mat Harris_Corner_Detector(QString f_name);

//Image Sharpening
Mat Image_Sharpening(QString f_name);

//Mat Image_Smoothing(QStringList f_lt)
Mat Image_Smoothing(QStringList f_lt);


//Mat Image_Stitching(QStringList f_lt)
Mat Image_Stitching(QStringList f_lt);


//Point Operations on Digital Images
Mat Point_Operations_on_Digital_Images(QString f_n);

//seam carving;
Mat compute_energy_matrix(Mat img);
Mat compute_score_matrix(Mat energy_matrix);
std::vector<int> get_seam(Mat score_matrix);
Mat remove_single_seam(Mat img, std::vector<int> low_energy_seam);
Mat add_single_seam(Mat img, std::vector<int> low_energy_seam);
Mat remove_all_seams(Mat img, int num_seams);
Mat add_all_seams(Mat img, int num_seams);
Mat seam_carving(Mat img, Size out_size);

Mat seam_carving(QString f_n);

//设置迭代条件
inline TermCriteria TC(int iters, double eps)
{
	return TermCriteria(TermCriteria::MAX_ITER + (eps > 0 ? TermCriteria::EPS : 0), iters, eps);
}


//MYMEMO:  Call back function
class CVFunction
{
public:
	CVFunction();
	//find all edge
	QImage find_edge(int max_thresh ,	int thresh ,Mat srcImage);
	//find circle and rectangle and 6-sides sharp
	QImage find_sharp(Mat srcImage);

	QImage find_sharp2(Mat srcImage);

	// Mat to image, CV_8UC3 format
	QImage Mat2QImage(const cv::Mat &mat);
	// Mat to image, CV_8UC1 format
	QImage Mat2QImage1(const cv::Mat &mat);

	void salt(Mat &image, int n);
	void colorReduce0(cv::Mat &image, int div);
	void sharpen(const cv::Mat &image, cv::Mat &result) ;
	void sharpen2D(const cv::Mat &image, cv::Mat &result);

	Mat HistogramRGB(QString file_name);

	Mat Histogram1D(QString file_name);

	void getText(Mat image);

	cv::Mat getHistogram(const cv::Mat &image);
	cv::Mat getHistogramImage(const cv::Mat &image);


	void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);

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


	std::vector<std::vector<cv::Point> > findSquaresInImage(cv::Mat _image);

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
