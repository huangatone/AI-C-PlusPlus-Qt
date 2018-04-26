#include "cvfunction.h"

#include <QtMath>

#include <iostream>
#include <stdio.h>

#include "cvfunction.h"
//#include <opencv2/tracking.hpp>
#include <QDebug>
#include <QMessageBox>

#include <map>
#include <vector>
#include <QFile>
#include <QDir>
#include "opencv2/text.hpp"

using namespace cv::text;
using namespace std;

/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

struct TextResult{
	double bi;
	int num;

};

struct TextCon{
	double x,y;                    //轮廓位置
	int order;                      //轮廓向量contours中的第几个

	bool operator<(TextCon &m){

	}

};
TextCon con[15];
TextResult result123[15];

bool bba(TextResult &b, TextResult &m){
	if(b.bi < m.bi)return true;
			else return false;
}


bool bbc(TextCon &b, TextCon &m){
	if(b.y > m.y) return false;
	else  if( b.y == m.y){
		if(b.x < m.x) return true;
		else return false;
	}
	else return true;
}

size_t min(size_t x, size_t y, size_t z)
{
	return x < y ? min(x,z) : min(y,z);
}

size_t edit_distance(const string& A, const string& B)
{
	size_t NA = A.size();
	size_t NB = B.size();

	vector< vector<size_t> > M(NA + 1, vector<size_t>(NB + 1));

	for (size_t a = 0; a <= NA; ++a)
		M[a][0] = a;

	for (size_t b = 0; b <= NB; ++b)
		M[0][b] = b;

	for (size_t a = 1; a <= NA; ++a)
		for (size_t b = 1; b <= NB; ++b)
		{
			size_t x = M[a-1][b] + 1;
			size_t y = M[a][b-1] + 1;
			size_t z = M[a-1][b-1] + (A[a-1] == B[b-1] ? 0 : 1);
			M[a][b] = min(x,y,z);
		}

	return M[A.size()][B.size()];
}

bool isRepetitive1(const string& s)
{
	int count = 0;
	for (int i=0; i<(int)s.size(); i++)
	{
		if ((s[i] == 'i') ||
				(s[i] == 'l') ||
				(s[i] == 'I'))
			count++;
	}
	if (count > ((int)s.size()+1)/2)
	{
		return true;
	}
	return false;
}


bool isRepetitive(const string& s)
{
	int count  = 0;
	int count2 = 0;
	int count3 = 0;
	int first=(int)s[0];
	int last=(int)s[(int)s.size()-1];
	for (int i=0; i<(int)s.size(); i++)
	{
		if ((s[i] == 'i') ||
				(s[i] == 'l') ||
				(s[i] == 'I'))
			count++;
		if((int)s[i]==first)
			count2++;
		if((int)s[i]==last)
			count3++;
	}
	if ((count > ((int)s.size()+1)/2) || (count2 == (int)s.size()) || (count3 > ((int)s.size()*2)/3))
	{
		return true;
	}

	return false;
}


void er_draw(vector<Mat> &channels, vector<vector<ERStat> > &regions, vector<Vec2i> group, Mat& segmentation)
{
	for (int r=0; r<(int)group.size(); r++)
	{
		ERStat er = regions[group[r][0]][group[r][1]];
		if (er.parent != NULL) // deprecate the root region
		{
			int newMaskVal = 255;
			int flags = 4 + (newMaskVal << 8) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;
			floodFill(channels[group[r][0]],segmentation,Point(er.pixel%channels[group[r][0]].cols,er.pixel/channels[group[r][0]].cols),
					  Scalar(255),0,Scalar(er.level),Scalar(0),flags);
		}
	}
}

bool   sort_by_lenght(const string &a, const string &b){return (a.size()>b.size());}

//ERStat extraction is done in parallel for different channels
class Parallel_extractCSER: public cv::ParallelLoopBody
{
private:
	vector<Mat> &channels;
	vector< vector<ERStat> > &regions;
	vector< Ptr<ERFilter> > er_filter1;
	vector< Ptr<ERFilter> > er_filter2;

public:
	Parallel_extractCSER(vector<Mat> &_channels, vector< vector<ERStat> > &_regions,
						 vector<Ptr<ERFilter> >_er_filter1, vector<Ptr<ERFilter> >_er_filter2)
		: channels(_channels),regions(_regions),er_filter1(_er_filter1),er_filter2(_er_filter2) {}

	virtual void operator()( const cv::Range &r ) const
	{
		for (int c=r.start; c < r.end; c++)
		{
			er_filter1[c]->run(channels[c], regions[c]);
			er_filter2[c]->run(channels[c], regions[c]);
		}
	}
	Parallel_extractCSER & operator=(const Parallel_extractCSER &a);
};
//OCR recognition is done in parallel for different detections
template <class T>
class Parallel_OCR: public cv::ParallelLoopBody
{
private:
	vector<Mat> &detections;
	vector<string> &outputs;
	vector< vector<Rect> > &boxes;
	vector< vector<string> > &words;
	vector< vector<float> > &confidences;
	vector< Ptr<T> > &ocrs;

public:
	Parallel_OCR(vector<Mat> &_detections, vector<string> &_outputs, vector< vector<Rect> > &_boxes,
				 vector< vector<string> > &_words, vector< vector<float> > &_confidences,
				 vector< Ptr<T> > &_ocrs)
		: detections(_detections), outputs(_outputs), boxes(_boxes), words(_words),
		  confidences(_confidences), ocrs(_ocrs)
	{}

	virtual void operator()( const cv::Range &r ) const
	{
		for (int c=r.start; c < r.end; c++)
		{
			ocrs[c%ocrs.size()]->run(detections[c], outputs[c], &boxes[c], &words[c], &confidences[c], OCR_LEVEL_WORD);
		}
	}
	Parallel_OCR & operator=(const Parallel_OCR &a);
};



void GetTextFromPicture(Mat frame)
{
	vector< Ptr<ERFilter> > er_filters1;
	   vector< Ptr<ERFilter> > er_filters2;
	 Mat  image, gray, out_img;
	 bool downsize = false;
		int  REGION_TYPE = 1;
		int  GROUPING_ALGORITHM = 0;
		int  RECOGNITION = 0;

		String region_types_str[2] = {"ERStats", "MSER"};
		String grouping_algorithms_str[2] = {"exhaustive_search", "multioriented"};
		String recognitions_str[2] = {"Tesseract", "NM_chain_features + KNN"};

		vector<Mat> channels;
		vector<vector<ERStat> > regions(2); //two channels


	int num_ocrs = 10;
	vector< Ptr<OCRTesseract> > ocrs;
	for (int o=0; o<num_ocrs; o++)
	{
		ocrs.push_back(OCRTesseract::create());
	}
	Mat transition_p;
	string filename = "OCRHMM_transitions_table.xml";
	FileStorage fs(filename, FileStorage::READ);
	fs["transition_probabilities"] >> transition_p;
	fs.release();
	Mat emission_p = Mat::eye(62,62,CV_64FC1);
	string voc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	vector< Ptr<OCRHMMDecoder> > decoders;
	for (int o=0; o<num_ocrs; o++)
	{
		decoders.push_back(OCRHMMDecoder::create(loadOCRHMMClassifierNM("/rong/lib/OCRHMM_knn_model_data.xml.gz"),
						   voc, transition_p, emission_p));
	}
	qDebug() << " Done!" << endl;


	/*Text Detection*/
			cvtColor(frame,gray,COLOR_BGR2GRAY);
			// Extract channels to be processed individually
			channels.clear();
			channels.push_back(gray);
			channels.push_back(255-gray);

			regions[0].clear();
			regions[1].clear();

			switch (REGION_TYPE)
			{
			case 0: // ERStats
				parallel_for_(cv::Range(0, (int)channels.size()), Parallel_extractCSER(channels, regions, er_filters1, er_filters2));
				break;
			case 1: // MSER
				vector<vector<Point> > contours;
				vector<Rect> bboxes;
				Ptr<MSER> mser = MSER::create(21, (int)(0.00002*gray.cols*gray.rows), (int)(0.05*gray.cols*gray.rows), 1, 0.7);
				mser->detectRegions(gray, contours, bboxes);

				//Convert the output of MSER to suitable input for the grouping/recognition algorithms
				if (contours.size() > 0)
					MSERsToERStats(gray, contours, regions);
				break;
			}

			// Detect character groups
			vector< vector<Vec2i> > nm_region_groups;
			vector<Rect> nm_boxes;
			switch (GROUPING_ALGORITHM)
			{
			case 0: // exhaustive_search
				erGrouping(frame, channels, regions, nm_region_groups, nm_boxes, ERGROUPING_ORIENTATION_HORIZ);
				break;
			case 1: //multioriented
				erGrouping(frame, channels, regions, nm_region_groups, nm_boxes, ERGROUPING_ORIENTATION_ANY, "./trained_classifier_erGrouping.xml", 0.5);
				break;
			}

			/*Text Recognition (OCR)*/

			int bottom_bar_height= out_img.rows/7 ;
			copyMakeBorder(frame, out_img, 0, bottom_bar_height, 0, 0, BORDER_CONSTANT, Scalar(150, 150, 150));
			float scale_font = (float)(bottom_bar_height /85.0);
			vector<string> words_detection;
			float min_confidence1 = 0.f, min_confidence2 = 0.f;

			if (RECOGNITION == 0)
			{
				min_confidence1 = 51.f;
				min_confidence2 = 60.f;
			}

			vector<Mat> detections;

			for (int i=0; i<(int)nm_boxes.size(); i++)
			{
				rectangle(out_img, nm_boxes[i].tl(), nm_boxes[i].br(), Scalar(255,255,0),3);

				Mat group_img = Mat::zeros(frame.rows+2, frame.cols+2, CV_8UC1);
				er_draw(channels, regions, nm_region_groups[i], group_img);
				group_img(nm_boxes[i]).copyTo(group_img);
				copyMakeBorder(group_img,group_img,15,15,15,15,BORDER_CONSTANT,Scalar(0));
				detections.push_back(group_img);
			}
			vector<string> outputs((int)detections.size());
			vector< vector<Rect> > boxes((int)detections.size());
			vector< vector<string> > words((int)detections.size());
			vector< vector<float> > confidences((int)detections.size());

			// parallel process detections in batches of ocrs.size() (== num_ocrs)
			for (int i=0; i<(int)detections.size(); i=i+(int)num_ocrs)
			{
				Range r;
				if (i+(int)num_ocrs <= (int)detections.size())
					r = Range(i,i+(int)num_ocrs);
				else
					r = Range(i,(int)detections.size());

				switch(RECOGNITION)
				{
				case 0: // Tesseract
					parallel_for_(r, Parallel_OCR<OCRTesseract>(detections, outputs, boxes, words, confidences, ocrs));
					break;
				case 1: // NM_chain_features + KNN
					parallel_for_(r, Parallel_OCR<OCRHMMDecoder>(detections, outputs, boxes, words, confidences, decoders));
					break;
				}
			}

			for (int i=0; i<(int)detections.size(); i++)
			{
				outputs[i].erase(remove(outputs[i].begin(), outputs[i].end(), '\n'), outputs[i].end());
				//qDebug() << "OCR output = \"" << outputs[i] << "\" length = " << outputs[i].size() << endl;
				if (outputs[i].size() < 3)
					continue;

				for (int j=0; j<(int)boxes[i].size(); j++)
				{
					boxes[i][j].x += nm_boxes[i].x-15;
					boxes[i][j].y += nm_boxes[i].y-15;

					//qDebug() << "  word = " << words[j] << "\t confidence = " << confidences[j] << endl;
					if ((words[i][j].size() < 2) || (confidences[i][j] < min_confidence1) ||
							((words[i][j].size()==2) && (words[i][j][0] == words[i][j][1])) ||
							((words[i][j].size()< 4) && (confidences[i][j] < min_confidence2)) ||
							isRepetitive(words[i][j]))
						continue;
					words_detection.push_back(words[i][j]);
					rectangle(out_img, boxes[i][j].tl(), boxes[i][j].br(), Scalar(255,0,255),3);
					Size word_size = getTextSize(words[i][j], FONT_HERSHEY_SIMPLEX, (double)scale_font, (int)(3*scale_font), NULL);
					rectangle(out_img, boxes[i][j].tl()-Point(3,word_size.height+3), boxes[i][j].tl()+Point(word_size.width,0), Scalar(255,0,255),-1);
					putText(out_img, words[i][j], boxes[i][j].tl()-Point(1,1), FONT_HERSHEY_SIMPLEX, scale_font, Scalar(255,255,255),(int)(3*scale_font));
				}
			}


			int text_thickness = 1+(out_img.rows/500);

			putText(out_img, "fps_info", Point( 10,out_img.rows-5 ), FONT_HERSHEY_DUPLEX, scale_font, Scalar(255,0,0), text_thickness);
			putText(out_img, region_types_str[REGION_TYPE], Point((int)(out_img.cols*0.5), out_img.rows - (int)(bottom_bar_height / 1.5)), FONT_HERSHEY_DUPLEX, scale_font, Scalar(255,0,0), text_thickness);
			putText(out_img, grouping_algorithms_str[GROUPING_ALGORITHM], Point((int)(out_img.cols*0.5),out_img.rows-((int)(bottom_bar_height /3)+4) ), FONT_HERSHEY_DUPLEX, scale_font, Scalar(255,0,0), text_thickness);
			putText(out_img, recognitions_str[RECOGNITION], Point((int)(out_img.cols*0.5),out_img.rows-5 ), FONT_HERSHEY_DUPLEX, scale_font, Scalar(255,0,0), text_thickness);

			imshow("recognition", out_img);


}

CVFunction::CVFunction()
{
	// Prepare arguments for 1D histogram
	histSize[0]= 256;
	hranges[0]= 0.0;
	hranges[1]= 255.0;
	ranges[0]= hranges;
	channels[0]= 0; // by default, we look at channel 0


	String face_cascade_name = "/rong/lib/opencv/3.4.0/share/opencv/haarcascades/haarcascade_frontalface_default.xml";
	String eyes_cascade_name = "/rong/lib/opencv/3.4.0/share/opencv/haarcascades/haarcascade_eye.xml";
	//-- 1. Load the cascades
	  if( !face_cascade.load( face_cascade_name ) )
	  {
		  printf("--(!)Error loading face cascade\n");
	  }
	  if( !eyes_cascade.load( eyes_cascade_name ) )
	  {
		  printf("--(!)Error loading eyes cascade\n");
	  }

}



//Mat srcImage = imread(file);
//ui->setPixmap( QPixmap::fromImage( find_edge(255,100,srcImage)));
QImage CVFunction::find_edge(int max_thresh ,	int thresh,Mat srcImage)
{
	RNG rng(12345);
	Mat srcGray;

	if (!srcImage.data)
		return QImage();
	cvtColor(srcImage, srcGray, COLOR_BGR2GRAY);
	blur(srcGray, srcGray, Size(3, 3));

	Mat canny_output;
    vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//用canny算子检测边缘
	Canny(srcGray, canny_output, thresh, thresh * 2, 3);
	//寻找轮廓
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//绘出轮廓
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255),rng.uniform(0,255),rng.uniform(0,255));
		//随机颜色绘制轮廓
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	return Mat2QImage(drawing);

}



Mat CVFunction::find_sharp(Mat srcImage)
{
	cv::Mat src= srcImage;
	cv::Mat gray;
	cv::Mat bw;
	cv::Mat dst;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Point> approx;
	RNG rng(12345);

	VideoCapture capture(0);

	// Convert to grayscale
	cv::cvtColor(src, gray, CV_BGR2GRAY);

	//阈值化操作 MYMEMO:good to have
	//threshold(srcGray, threMat, thresh, 255, THRESH_BINARY);

	// Use Canny instead of threshold to catch squares with gradient shading
	blur( gray, bw, Size(3,3) );
	cv::Canny(gray, bw, 80, 240, 3);

	//cv::bitwise_not(bw, bw);

	// Find contours
	cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//src.copyTo(dst);
	dst = Mat::zeros(src.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects
		qDebug() << std::fabs(cv::contourArea(contours[i])) ;
		if (std::fabs(cv::contourArea(contours[i])) < 500 || !cv::isContourConvex(approx))
			continue;

		if (approx.size() == 3)
		{
			setLabel(dst, "TRI", contours[i]);    // Triangles
		}
		else if (approx.size() >= 4 && approx.size() <= 6)
		{
			// Number of vertices of polygonal curve
			int vtc = approx.size();

			// Get the cosines of all corners
			std::vector<double> cos;
			for (int j = 2; j < vtc+1; j++)
				cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

			// Sort ascending the cosine values
			std::sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Use the degrees obtained above and the number of vertices
			// to determine the shape of the contour
			if (vtc == 4 )
				setLabel(dst, "RECT", contours[i]);
			else if (vtc == 5 )
				setLabel(dst, "PENTA", contours[i]);
			else if (vtc == 6 )
				setLabel(dst, "HEXA", contours[i]);

			Scalar color = Scalar(rng.uniform(0, 255),rng.uniform(0,255),rng.uniform(0,255));
			//随机颜色绘制轮廓
			drawContours(dst, contours, i, color);

		}
		else
		{
			// Detect and label circles
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = r.width / 2;

			if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
				std::abs(1 - (area / (CV_PI * (radius*radius)))) <= 0.2)
				setLabel(dst, "CIR", contours[i]);
		}
	}

	return dst;

}


Mat CVFunction::find_sharp2(Mat srcImage)
{
	qDebug() << "sharp2";
	Mat srcTemp = srcImage.clone();
	cv::Mat srcGray;
	cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	blur(srcGray, srcGray, Size(3, 3));

	Mat2QImage1(srcGray).save("gray.png");

	int thresh = 100;
	int max_thresh = 255;
	RNG rng(12345);

	Mat threMat;
	//轮廓检测参数
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//阈值化操作
	threshold(srcGray, threMat, thresh, max_thresh, THRESH_BINARY);
	//轮廓检测
	findContours(threMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//凸包及缺陷检测参数
	Mat2QImage1(threMat).save("threMat.png");

	vector<vector<Point> > pointHull(contours.size());
	vector<vector<int> > intHull(contours.size());
	vector<vector<Vec4i> > hullDefect(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		//Point类型凸包检测
		convexHull(Mat(contours[i]), pointHull[i], false);
		//int 类型凸包检测
		convexHull(Mat(contours[i]), intHull[i], false);
		//凸包缺陷检测
		convexityDefects(Mat(contours[i]), intHull[i], hullDefect[i]);
	}
	//绘制凸包及缺陷检测
	Mat drawing = Mat::zeros(threMat.size(), CV_8UC1);
	Mat2QImage1(drawing).save("drawing.png");

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(drawing, pointHull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		//绘制缺陷
		size_t count = contours[i].size();
		if (count < 300)
			continue;
		//设置凸包缺陷迭代器
		vector<Vec4i>::iterator iterDefects = hullDefect[i].begin();
		//遍历得到4个特征量
		while (iterDefects != hullDefect[i].end())
		{
			Vec4i& v = (*iterDefects);
			//起始位置
			int startidx = v[0];
			Point ptStart(contours[i][startidx]);
			//终止位置
			int endidx = v[1];
			Point ptEnd(contours[i][endidx]);
			//内凸壳最远的点缺陷
			int faridx = v[2];
			Point ptFar(contours[i][faridx]);
			//凸点之间的最远点
			int depth = v[3] / 256;
			//绘制相应的线与圆检测结果
			if (depth > 20 && depth < 80)
			{
				line(drawing, ptStart, ptFar, CV_RGB(0, 255, 0), 2);
				line(drawing, ptEnd, ptFar, CV_RGB(0, 255, 0), 2);
				circle(drawing, ptStart, 4, Scalar(255, 0, 100), 2);
				circle(drawing, ptEnd, 4, Scalar(255, 0, 100), 2);
				circle(drawing, ptFar, 4, Scalar(100, 0, 255), 2);
			}
			iterDefects++;
		}
	}
qDebug() << "sharp2 end";
//imshow("result", drawing);

//todo: return threright picture;
	return drawing;
}


QImage CVFunction::Mat2QImage(const cv::Mat &mat)
{
	QImage img;
	if(mat.channels()==3)
	{
		//cvt Mat BGR 2 QImage RGB
		cvtColor(mat,rgb,CV_BGR2RGB);
		img =QImage((const unsigned char*)(rgb.data),
					rgb.cols,rgb.rows,
					rgb.cols*rgb.channels(),
					QImage::Format_RGB888);
	}
	else
	{
		img =QImage((const unsigned char*)(mat.data),
					mat.cols,mat.rows,
					mat.cols*mat.channels(),
					QImage::Format_RGB888);
	}
	return img;
}

QImage CVFunction::Mat2QImage1(const Mat &result)
{
	//BGR openCV Mat to QImage
	QImage img_qt = QImage((const unsigned char*)result.data,result.cols, result.rows, result.step, QImage::Format_RGB888);

	//For Binary Images
	if (img_qt.isNull())
	{
		//ColorTable for Binary Images
		QVector<QRgb> colorTable;
		for (int i = 0; i < 256; i++)
			colorTable.push_back(qRgb(i, i, i));

		img_qt = QImage((const unsigned char*)result.data,result.cols, result.rows, QImage::Format_Indexed8);
		img_qt.setColorTable(colorTable);
	}

	return img_qt.rgbSwapped();
}


void CVFunction::salt(Mat &image, int n)
{

	int i,j;
	for (int k=0; k<n; k++)
	{

		i= qrand()%image.cols;
		j= qrand()%image.rows;
		//qDebug() << i << j;

		if (image.channels() == 1) { // gray-level image

			image.at<uchar>(j,i)= 255;

		} else if (image.channels() == 3) { // color image

			image.at<cv::Vec3b>(j,i)[0]= 255;
			image.at<cv::Vec3b>(j,i)[1]= 255;
			image.at<cv::Vec3b>(j,i)[2]= 255;
		}
	}
}

//-----------------Above Done--------Below Investigation-----------------------------------//
// using .ptr and []
void CVFunction::colorReduce0(cv::Mat &image, int div)
{

	  int nl= image.rows; // number of lines
	  int nc= image.cols * image.channels(); // total number of elements per line

	  for (int j=0; j<nl; j++)
	  {

		  uchar* data= image.ptr<uchar>(j);

		  for (int i=0; i<nc; i++)
		  {

			// process each pixel ---------------------
				data[i]= data[i]/div*div+div/2;

			// end of pixel processing ----------------

		  } // end of line
	  }
}


void CVFunction::sharpen(const cv::Mat &image, cv::Mat &result)
{
	// allocate if necessary
	result.create(image.size(), image.type());
	for (int j= 1; j<image.rows-1; j++)
	{ // for all rows
		// (except first and last)
		const uchar* previous=		   image.ptr<const uchar>(j-1); // previous row
		const uchar* current =		   image.ptr<const uchar>(j);     // current row
		const uchar* next    =		   image.ptr<const uchar>(j+1); // next row
		uchar* output= result.ptr<uchar>(j); // output row
		for (int i=1; i<image.cols-1; i++)
		{
		   *output++= cv::saturate_cast<uchar>(	 5*current[i] - current[i-1] -current[i+1]-previous[i]-next[i]);
		}
	}
	// Set the unprocess pixels to 0
	result.row(0).setTo(cv::Scalar(0));
	result.row(result.rows-1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(result.cols-1).setTo(cv::Scalar(0));
}

void CVFunction::sharpen2D(const cv::Mat &image, cv::Mat &result) {
   // Construct kernel (all entries initialized to 0)
   cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));
   // assigns kernel values
   kernel.at<float>(1,1)= 5.0;
   kernel.at<float>(0,1)= -1.0;
   kernel.at<float>(2,1)= -1.0;
   kernel.at<float>(1,0)= -1.0;
   kernel.at<float>(1,2)= -1.0;
   //filter the image
   cv::filter2D(image,result,image.depth(),kernel);
}


Mat CVFunction::HistogramRGB(QString file_name)
{
	Mat src, dst;

	  /// Load image
	  src = imread( file_name.toStdString(), 1 );

	  if( !src.data )
	  {
		  return dst;
	  }

	  /// Separate the image in 3 places ( B, G and R )
	  vector<Mat> bgr_planes;
	  split( src, bgr_planes );

	  /// Establish the number of bins
	  int histSize = 256;

	  /// Set the ranges ( for B,G,R) )
	  float range[] = { 0, 256 } ;
	  const float* histRange = { range };

	  bool uniform = true;
	  bool accumulate = false;

	  Mat b_hist, g_hist, r_hist;

	  /// Compute the histograms:
	  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

	  // Draw the histograms for B, G and R
	  int hist_w = 512; int hist_h = 400;
	  int bin_w = cvRound( (double) hist_w/histSize );

	  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	  /// Normalize the result to [ 0, histImage.rows ]
	  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	  /// Draw for each channel
	  for( int i = 1; i < histSize; i++ )
	  {
		  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
						   Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
						   Scalar( 255, 0, 0), 2, 8, 0  );
		  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
						   Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
						   Scalar( 0, 255, 0), 2, 8, 0  );
		  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
						   Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
						   Scalar( 0, 0, 255), 2, 8, 0  );
	  }

	  return histImage;
}

Mat CVFunction::Histogram1D(QString file_name)
{
	Mat src, dst;

	  /// Load image
	  src = imread( file_name.toStdString(), 0 );

	  if( !src.data )
	  {
		  return dst;
	  }

	  /// Separate the image in 3 places ( B, G and R )
	  vector<Mat> bgr_planes;
	  split( src, bgr_planes );

	  /// Establish the number of bins
	  int histSize = 256;

	  /// Set the ranges ( for B,G,R) )
	  float range[] = { 0, 256 } ;
	  const float* histRange = { range };

	  bool uniform = true;
	  bool accumulate = false;

	  Mat b_hist;

	  /// Compute the histograms:
	  ///  // Compute histogram
	/*  cv::calcHist(&image,
		 1,           // histogram from 1 image only
		 channels,  // the channel used
		 cv::Mat(), // no mask is used
		 hist,        // the resulting histogram
		 1,           // it is a 1D histogram
		 histSize,  // number of bins
		 ranges     // pixel value range
);
	  */

	  calcHist( &src, 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );

	  return b_hist;
}


// Computes the 1D histogram.
cv::Mat CVFunction::getHistogram(const cv::Mat &image)
{
	cv::MatND hist;
	// Compute histogram
	cv::calcHist(&image,
					1,           // histogram from 1 image only
					channels,  // the channel used
					cv::Mat(), // no mask is used
					hist,        // the resulting histogram
					1,           // it is a 1D histogram
					histSize,  // number of bins
					ranges     // pixel value range
	);
	return hist;
}


// Computes the 1D histogram and returns an image of it.
cv::Mat CVFunction::getHistogramImage(const cv::Mat &image)
{
	// Compute histogram first
	cv::MatND hist= getHistogram(image);
	// Get min and max bin values
	double maxVal=0;
	double minVal=0;
	cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);
	// Image on which to display histogram
	cv::Mat histImg(histSize[0], histSize[0],
	CV_8U,cv::Scalar(255));
	// set highest point at 90% of nbins
	int hpt = static_cast<int>(0.9*histSize[0]);
	// Draw a vertical line for each bin
	for( int h = 0; h < histSize[0]; h++ )
	{
		float binVal = hist.at<float>(h);
		int intensity = static_cast<int>(binVal*hpt/maxVal);
		// This function draws a line between 2 points
		cv::line(histImg,cv::Point(h,histSize[0]),cv::Point(h,histSize[0]-intensity),
				cv::Scalar::all(0));
	}
	return histImg;
}


void Fun1()
{
	// Read reference image
	cv::Mat image= cv::imread("../baboon1.jpg");
	// Baboon's face ROI
	cv::Mat imageROI= image(cv::Rect(110,260,35,40));
	// Get the Hue histogram
	int minSat=65;
	//ColorHistogram hc;
	//cv::MatND colorhist=   hc.getHueHistogram(imageROI,minSat);
}


/** @function detectAndDisplay */





/**
 * Helper function to display text in the center of a contour
 */
void CVFunction::setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

Mat CVFunction::face_detect(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

	for( size_t i = 0; i < faces.size(); i++ )
	{
		Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
		ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

		Mat faceROI = frame_gray( faces[i] );
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );

		for( size_t j = 0; j < eyes.size(); j++ )
		{
			Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
			int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
			circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
		}
	}
	return  frame;
}

Mat CVFunction::sharp_detect(Mat &img)
{
	cv::Mat src = img;
	cv::Mat gray;
	cv::Mat bw;
	cv::Mat dst;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Point> approx;

	// Convert to grayscale
	cv::cvtColor(src, gray, CV_BGR2GRAY);

	// Use Canny instead of threshold to catch squares with gradient shading
	blur( gray, bw, Size(3,3) );
	cv::Canny(gray, bw, 80, 240, 3);

	//cv::bitwise_not(bw, bw);

	// Find contours
	cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	src.copyTo(dst);

	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
			continue;

		if (approx.size() == 3)
		{
			setLabel(dst, "TRI", contours[i]);    // Triangles
		}
		else if (approx.size() >= 4 && approx.size() <= 6)
		{
			// Number of vertices of polygonal curve
			int vtc = approx.size();

			// Get the cosines of all corners
			std::vector<double> cos;
			for (int j = 2; j < vtc+1; j++)
				cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

			// Sort ascending the cosine values
			std::sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Use the degrees obtained above and the number of vertices
			// to determine the shape of the contour
			if (vtc == 4 )
				setLabel(dst, "RECT", contours[i]);
			else if (vtc == 5 )
				setLabel(dst, "PENTA", contours[i]);
			else if (vtc == 6 )
				setLabel(dst, "HEXA", contours[i]);
		}
		else
		{
			// Detect and label circles
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = r.width / 2;

			if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
				std::abs(1 - (area / (CV_PI * (radius*radius)))) <= 0.2)
				setLabel(dst, "CIR", contours[i]);
		}
	}
	return dst;
}

Mat CVFunction::text_detect( Mat image)
{


	qDebug() << "A demo program of Scene Text Character Recognition: " << endl;
	qDebug() << "Shows the use of the OCRBeamSearchDecoder::ClassifierCallback class using the Single Layer CNN character classifier described in:" << endl;
	qDebug() << "Coates, Adam, et al. \"Text detection and character recognition in scene images with unsupervised feature learning.\" ICDAR 2011." << endl << endl;

	string vocabulary = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; // must have the same order as the classifier output classes

	Ptr<OCRHMMDecoder::ClassifierCallback> ocr = loadOCRHMMClassifierCNN("/rong/lib/OCRBeamSearch_CNN_model_data.xml.gz");

	double t_r = (double)getTickCount();
	vector<int> out_classes;
	vector<double> out_confidences;

	ocr->eval(image, out_classes, out_confidences);

	qDebug() << "OCR output = \"" << vocabulary[out_classes[0]] << "\" with confidence "
		 << out_confidences[0] << ". Evaluated in "
		 << ((double)getTickCount() - t_r)*1000/getTickFrequency() << " ms." << endl << endl;
qDebug() << "OCR output = \"" << QString::fromStdString( vocabulary);
	return image;
}


//Perform text detection and recognition and evaluate results using edit distance
int CVFunction::text_detect2(Mat image)
{

	qDebug() << "A demo program of End-to-end Scene Text Detection and Recognition: " << endl;
	qDebug() << "Shows the use of the Tesseract OCR API with the Extremal Region Filter algorithm described in:" << endl;
	qDebug() << "Neumann L., Matas J.: Real-Time Scene Text Localization and Recognition, CVPR 2012" << endl << endl;

	qDebug() << "IMG_W=" << image.cols << endl;
	qDebug() << "IMG_H=" << image.rows << endl;

	/*Text Detection*/

	// Extract channels to be processed individually
	vector<Mat> channels;

	Mat grey;
	cvtColor(image,grey,COLOR_RGB2GRAY);

	// Notice here we are only using grey channel, see textdetection.cpp for example with more channels
	channels.push_back(grey);
	channels.push_back(255-grey);

	double t_d = (double)getTickCount();
	// Create ERFilter objects with the 1st and 2nd stage default classifiers
	Ptr<ERFilter> er_filter1 = createERFilterNM1(loadClassifierNM1("/rong/lib/trained_classifierNM1.xml"),8,0.00015f,0.13f,0.2f,true,0.1f);
	Ptr<ERFilter> er_filter2 = createERFilterNM2(loadClassifierNM2("/rong/lib/trained_classifierNM2.xml"),0.5);

	vector<vector<ERStat> > regions(channels.size());
	// Apply the default cascade classifier to each independent channel (could be done in parallel)
	for (int c=0; c<(int)channels.size(); c++)
	{
		er_filter1->run(channels[c], regions[c]);
		er_filter2->run(channels[c], regions[c]);
	}
	qDebug() << "TIME_REGION_DETECTION = " << ((double)getTickCount() - t_d)*1000/getTickFrequency() << endl;

	Mat out_img_decomposition= Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
	vector<Vec2i> tmp_group;
	for (int i=0; i<(int)regions.size(); i++)
	{
		for (int j=0; j<(int)regions[i].size();j++)
		{
			tmp_group.push_back(Vec2i(i,j));
		}
		Mat tmp= Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
		er_draw(channels, regions, tmp_group, tmp);
		if (i > 0)
			tmp = tmp / 2;
		out_img_decomposition = out_img_decomposition | tmp;
		tmp_group.clear();
	}

	double t_g = (double)getTickCount();
	// Detect character groups
	vector< vector<Vec2i> > nm_region_groups;
	vector<Rect> nm_boxes;
	erGrouping(image, channels, regions, nm_region_groups, nm_boxes,ERGROUPING_ORIENTATION_HORIZ);
	qDebug() << "TIME_GROUPING = " << ((double)getTickCount() - t_g)*1000/getTickFrequency() << endl;



	/*Text Recognition (OCR)*/

	double t_r = (double)getTickCount();
	Ptr<OCRTesseract> ocr = OCRTesseract::create();
	qDebug() << "TIME_OCR_INITIALIZATION = " << ((double)getTickCount() - t_r)*1000/getTickFrequency() << endl;
	string output;

	Mat out_img;
	Mat out_img_detection;
	Mat out_img_segmentation = Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
	image.copyTo(out_img);
	image.copyTo(out_img_detection);
	float scale_img  = 600.f/image.rows;
	float scale_font = (float)(2-scale_img)/1.4f;
	vector<string> words_detection;

	t_r = (double)getTickCount();

	for (int i=0; i<(int)nm_boxes.size(); i++)
	{

		rectangle(out_img_detection, nm_boxes[i].tl(), nm_boxes[i].br(), Scalar(0,255,255), 3);

		Mat group_img = Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
		er_draw(channels, regions, nm_region_groups[i], group_img);
		Mat group_segmentation;
		group_img.copyTo(group_segmentation);
		//image(nm_boxes[i]).copyTo(group_img);
		group_img(nm_boxes[i]).copyTo(group_img);
		copyMakeBorder(group_img,group_img,15,15,15,15,BORDER_CONSTANT,Scalar(0));

		vector<Rect>   boxes;
		vector<string> words;
		vector<float>  confidences;
		ocr->run(group_img, output, &boxes, &words, &confidences, OCR_LEVEL_WORD);

		output.erase(remove(output.begin(), output.end(), '\n'), output.end());
		//qDebug() << "OCR output = \"" << output << "\" length = " << output.size() << endl;
		if (output.size() < 3)
			continue;

		for (int j=0; j<(int)boxes.size(); j++)
		{
			boxes[j].x += nm_boxes[i].x-15;
			boxes[j].y += nm_boxes[i].y-15;

			//qDebug() << "  word = " << words[j] << "\t confidence = " << confidences[j] << endl;
			if ((words[j].size() < 2) || (confidences[j] < 51) ||
					((words[j].size()==2) && (words[j][0] == words[j][1])) ||
					((words[j].size()< 4) && (confidences[j] < 60)) ||
					isRepetitive(words[j]))
				continue;
			words_detection.push_back(words[j]);
			rectangle(out_img, boxes[j].tl(), boxes[j].br(), Scalar(255,0,255),3);
			Size word_size = getTextSize(words[j], FONT_HERSHEY_SIMPLEX, (double)scale_font, (int)(3*scale_font), NULL);
			rectangle(out_img, boxes[j].tl()-Point(3,word_size.height+3), boxes[j].tl()+Point(word_size.width,0), Scalar(255,0,255),-1);
			putText(out_img, words[j], boxes[j].tl()-Point(1,1), FONT_HERSHEY_SIMPLEX, scale_font, Scalar(255,255,255),(int)(3*scale_font));
			out_img_segmentation = out_img_segmentation | group_segmentation;
		}

	}

	qDebug() << "TIME_OCR = " << ((double)getTickCount() - t_r)*1000/getTickFrequency() << endl;


	/* Recognition evaluation with (approximate) Hungarian matching and edit distances */


	//resize(out_img_detection,out_img_detection,Size(image.cols*scale_img,image.rows*scale_img),0,0,INTER_LINEAR_EXACT);
	//imshow("detection", out_img_detection);
	//imwrite("detection.jpg", out_img_detection);
	//resize(out_img,out_img,Size(image.cols*scale_img,image.rows*scale_img),0,0,INTER_LINEAR_EXACT);
	namedWindow("recognition",WINDOW_NORMAL);
	imshow("recognition", out_img);
	//waitKey(0);
	//imwrite("recognition.jpg", out_img);
	//imwrite("segmentation.jpg", out_img_segmentation);
	//imwrite("decomposition.jpg", out_img_decomposition);

	return 0;
}

void CVFunction::getText(Mat image)
{

		cout << "A demo program of End-to-end Scene Text Detection and Recognition: " << endl;
		cout << "Shows the use of the Tesseract OCR API with the Extremal Region Filter algorithm described in:" << endl;
		cout << "Neumann L., Matas J.: Real-Time Scene Text Localization and Recognition, CVPR 2012" << endl << endl;

		cout << "IMG_W=" << image.cols << endl;
		cout << "IMG_H=" << image.rows << endl;

		/*Text Detection*/

		// Extract channels to be processed individually
		vector<Mat> channels;

		Mat grey;
		cvtColor(image,grey,COLOR_RGB2GRAY);

		// Notice here we are only using grey channel, see textdetection.cpp for example with more channels
		channels.push_back(grey);
		channels.push_back(255-grey);

		double t_d = (double)getTickCount();
		// Create ERFilter objects with the 1st and 2nd stage default classifiers
		Ptr<ERFilter> er_filter1 = createERFilterNM1(loadClassifierNM1("trained_classifierNM1.xml"),8,0.00015f,0.13f,0.2f,true,0.1f);
		Ptr<ERFilter> er_filter2 = createERFilterNM2(loadClassifierNM2("trained_classifierNM2.xml"),0.5);

		vector<vector<ERStat> > regions(channels.size());
		// Apply the default cascade classifier to each independent channel (could be done in parallel)
		for (int c=0; c<(int)channels.size(); c++)
		{
			er_filter1->run(channels[c], regions[c]);
			er_filter2->run(channels[c], regions[c]);
		}
		cout << "TIME_REGION_DETECTION = " << ((double)getTickCount() - t_d)*1000/getTickFrequency() << endl;

		Mat out_img_decomposition= Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
		vector<Vec2i> tmp_group;
		for (int i=0; i<(int)regions.size(); i++)
		{
			for (int j=0; j<(int)regions[i].size();j++)
			{
				tmp_group.push_back(Vec2i(i,j));
			}
			Mat tmp= Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
			er_draw(channels, regions, tmp_group, tmp);
			if (i > 0)
				tmp = tmp / 2;
			out_img_decomposition = out_img_decomposition | tmp;
			tmp_group.clear();
		}

		double t_g = (double)getTickCount();
		// Detect character groups
		vector< vector<Vec2i> > nm_region_groups;
		vector<Rect> nm_boxes;
		erGrouping(image, channels, regions, nm_region_groups, nm_boxes,ERGROUPING_ORIENTATION_HORIZ);
		cout << "TIME_GROUPING = " << ((double)getTickCount() - t_g)*1000/getTickFrequency() << endl;



		/*Text Recognition (OCR)*/

		double t_r = (double)getTickCount();
		Ptr<OCRTesseract> ocr = OCRTesseract::create();
		cout << "TIME_OCR_INITIALIZATION = " << ((double)getTickCount() - t_r)*1000/getTickFrequency() << endl;
		string output;

		Mat out_img;
		Mat out_img_detection;
		Mat out_img_segmentation = Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
		image.copyTo(out_img);
		image.copyTo(out_img_detection);
		float scale_img  = 600.f/image.rows;
		float scale_font = (float)(2-scale_img)/1.4f;
		vector<string> words_detection;

		t_r = (double)getTickCount();

		for (int i=0; i<(int)nm_boxes.size(); i++)
		{

			rectangle(out_img_detection, nm_boxes[i].tl(), nm_boxes[i].br(), Scalar(0,255,255), 3);

			Mat group_img = Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
			er_draw(channels, regions, nm_region_groups[i], group_img);
			Mat group_segmentation;
			group_img.copyTo(group_segmentation);
			//image(nm_boxes[i]).copyTo(group_img);
			group_img(nm_boxes[i]).copyTo(group_img);
			copyMakeBorder(group_img,group_img,15,15,15,15,BORDER_CONSTANT,Scalar(0));

			vector<Rect>   boxes;
			vector<string> words;
			vector<float>  confidences;
			ocr->run(group_img, output, &boxes, &words, &confidences, OCR_LEVEL_WORD);

			output.erase(remove(output.begin(), output.end(), '\n'), output.end());
			//cout << "OCR output = \"" << output << "\" length = " << output.size() << endl;
			if (output.size() < 3)
				continue;

			for (int j=0; j<(int)boxes.size(); j++)
			{
				boxes[j].x += nm_boxes[i].x-15;
				boxes[j].y += nm_boxes[i].y-15;

				//cout << "  word = " << words[j] << "\t confidence = " << confidences[j] << endl;
				if ((words[j].size() < 2) || (confidences[j] < 51) ||
						((words[j].size()==2) && (words[j][0] == words[j][1])) ||
						((words[j].size()< 4) && (confidences[j] < 60)) ||
						isRepetitive(words[j]))
					continue;
				words_detection.push_back(words[j]);
				rectangle(out_img, boxes[j].tl(), boxes[j].br(), Scalar(255,0,255),3);
				Size word_size = getTextSize(words[j], FONT_HERSHEY_SIMPLEX, (double)scale_font, (int)(3*scale_font), NULL);
				rectangle(out_img, boxes[j].tl()-Point(3,word_size.height+3), boxes[j].tl()+Point(word_size.width,0), Scalar(255,0,255),-1);
				putText(out_img, words[j], boxes[j].tl()-Point(1,1), FONT_HERSHEY_SIMPLEX, scale_font, Scalar(255,255,255),(int)(3*scale_font));
				out_img_segmentation = out_img_segmentation | group_segmentation;
			}

		}

		cout << "TIME_OCR = " << ((double)getTickCount() - t_r)*1000/getTickFrequency() << endl;


		/* Recognition evaluation with (approximate) Hungarian matching and edit distances */

		if(1)
		{
			int num_gt_characters   = 0;
			vector<string> words_gt;

			words_gt.push_back( "abcde");
			//cout << " GT word " << words_gt[words_gt.size()-1] << endl;
			num_gt_characters += 5;

			if (words_detection.empty())
			{
				//cout << endl << "number of characters in gt = " << num_gt_characters << endl;
				cout << "TOTAL_EDIT_DISTANCE = " << num_gt_characters << endl;
				cout << "EDIT_DISTANCE_RATIO = 1" << endl;
			}
			else
			{

				sort(words_gt.begin(),words_gt.end(),sort_by_lenght);

				int max_dist=0;
				vector< vector<int> > assignment_mat;
				for (int i=0; i<(int)words_gt.size(); i++)
				{
					vector<int> assignment_row(words_detection.size(),0);
					assignment_mat.push_back(assignment_row);
					for (int j=0; j<(int)words_detection.size(); j++)
					{
						assignment_mat[i][j] = (int)(edit_distance(words_gt[i],words_detection[j]));
						max_dist = max(max_dist,assignment_mat[i][j]);
					}
				}

				vector<int> words_detection_matched;

				int total_edit_distance = 0;
				int tp=0, fp=0, fn=0;
				for (int search_dist=0; search_dist<=max_dist; search_dist++)
				{
					for (int i=0; i<(int)assignment_mat.size(); i++)
					{
						int min_dist_idx =  (int)distance(assignment_mat[i].begin(),
											min_element(assignment_mat[i].begin(),assignment_mat[i].end()));
						if (assignment_mat[i][min_dist_idx] == search_dist)
						{
							//cout << " GT word \"" << words_gt[i] << "\" best match \"" << words_detection[min_dist_idx] << "\" with dist " << assignment_mat[i][min_dist_idx] << endl;
							if(search_dist == 0)
								tp++;
							else { fp++; fn++; }

							total_edit_distance += assignment_mat[i][min_dist_idx];
							words_detection_matched.push_back(min_dist_idx);
							words_gt.erase(words_gt.begin()+i);
							assignment_mat.erase(assignment_mat.begin()+i);
							for (int j=0; j<(int)assignment_mat.size(); j++)
							{
								assignment_mat[j][min_dist_idx]=INT_MAX;
							}
							i--;
						}
					}
				}

				for (int j=0; j<(int)words_gt.size(); j++)
				{
					//cout << " GT word \"" << words_gt[j] << "\" no match found" << endl;
					fn++;
					total_edit_distance += (int)words_gt[j].size();
				}
				for (int j=0; j<(int)words_detection.size(); j++)
				{

				}


				//cout << endl << "number of characters in gt = " << num_gt_characters << endl;
				cout << "TOTAL_EDIT_DISTANCE = " << total_edit_distance << endl;
				cout << "EDIT_DISTANCE_RATIO = " << (float)total_edit_distance / num_gt_characters << endl;
				cout << "TP = " << tp << endl;
				cout << "FP = " << fp << endl;
				cout << "FN = " << fn << endl;
			}
		}




		//resize(out_img_detection,out_img_detection,Size(image.cols*scale_img,image.rows*scale_img),0,0,INTER_LINEAR_EXACT);
		//imshow("detection", out_img_detection);
		//imwrite("detection.jpg", out_img_detection);
		//resize(out_img,out_img,Size(image.cols*scale_img,image.rows*scale_img),0,0,INTER_LINEAR_EXACT);
		namedWindow("recognition",WINDOW_NORMAL);
		imshow("recognition", out_img);
		waitKey(0);
}



int CVFunction::polygonfind( void )
{
  /// Create an image
  const int r = 100;
  Mat src = Mat::zeros( Size( 4*r, 4*r ), CV_8UC1 );

  /// Create a sequence of points to make a contour:
  vector<Point2f> vert(6);

  vert[0] = Point( 3*r/2, static_cast<int>(1.34*r) );
  vert[1] = Point( 1*r, 2*r );
  vert[2] = Point( 3*r/2, static_cast<int>(2.866*r) );
  vert[3] = Point( 5*r/2, static_cast<int>(2.866*r) );
  vert[4] = Point( 3*r, 2*r );
  vert[5] = Point( 5*r/2, static_cast<int>(1.34*r) );

  /// Draw it in src
  for( int j = 0; j < 6; j++ )
	 { line( src, vert[j],  vert[(j+1)%6], Scalar( 255 ), 3, 8 ); }

  /// Get the contours
  vector<vector<Point> > contours;

  findContours( src, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

  /// Calculate the distances to the contour
  Mat raw_dist( src.size(), CV_32FC1 );

  for( int j = 0; j < src.rows; j++ )
	 { for( int i = 0; i < src.cols; i++ )
		  { raw_dist.at<float>(j,i) = (float)pointPolygonTest( contours[0], Point2f((float)i,(float)j), true ); }
	 }

  double minVal; double maxVal;
  minMaxLoc( raw_dist, &minVal, &maxVal, 0, 0, Mat() );
  minVal = abs(minVal); maxVal = abs(maxVal);

  /// Depicting the  distances graphically
  Mat drawing = Mat::zeros( src.size(), CV_8UC3 );

  for( int j = 0; j < src.rows; j++ )
	 { for( int i = 0; i < src.cols; i++ )
		  {
			if( raw_dist.at<float>(j,i) < 0 )
			  { drawing.at<Vec3b>(j,i)[0] = (uchar)(255 - abs(raw_dist.at<float>(j,i))*255/minVal); }
			else if( raw_dist.at<float>(j,i) > 0 )
			  { drawing.at<Vec3b>(j,i)[2] = (uchar)(255 - raw_dist.at<float>(j,i)*255/maxVal); }
			else
			  { drawing.at<Vec3b>(j,i)[0] = 255; drawing.at<Vec3b>(j,i)[1] = 255; drawing.at<Vec3b>(j,i)[2] = 255; }
		  }
	 }

  /// Show your results
  imshow( "Source", src );
  imshow( "Distance", drawing );

  waitKey(0);
  return(0);
}



int CVFunction::polygonfind(  Mat image )
{
  /// Create an image

  Mat src = image;
  /// Get the contours
  vector<vector<Point> > contours;

  findContours( src, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

  /// Calculate the distances to the contour
  Mat raw_dist( src.size(), CV_32FC1 );

  for( int j = 0; j < src.rows; j++ )
	 { for( int i = 0; i < src.cols; i++ )
		  { raw_dist.at<float>(j,i) = (float)pointPolygonTest( contours[0], Point2f((float)i,(float)j), true ); }
	 }

  double minVal; double maxVal;
  minMaxLoc( raw_dist, &minVal, &maxVal, 0, 0, Mat() );
  minVal = abs(minVal); maxVal = abs(maxVal);

  /// Depicting the  distances graphically
  Mat drawing = Mat::zeros( src.size(), CV_8UC3 );

  for( int j = 0; j < src.rows; j++ )
	 { for( int i = 0; i < src.cols; i++ )
		  {
			if( raw_dist.at<float>(j,i) < 0 )
			  { drawing.at<Vec3b>(j,i)[0] = (uchar)(255 - abs(raw_dist.at<float>(j,i))*255/minVal); }
			else if( raw_dist.at<float>(j,i) > 0 )
			  { drawing.at<Vec3b>(j,i)[2] = (uchar)(255 - raw_dist.at<float>(j,i)*255/maxVal); }
			else
			  { drawing.at<Vec3b>(j,i)[0] = 255; drawing.at<Vec3b>(j,i)[1] = 255; drawing.at<Vec3b>(j,i)[2] = 255; }
		  }
	 }

  /// Show your results
  imshow( "Source", src );
  imshow( "Distance", drawing );

  waitKey(0);
  return(0);
}

int* findRectInfo(std::vector<cv::Point> rect)
{
	int rectInfo[4] = {0};
	int x[4]= {0},y[4]= {0};
	int maxX = 0,maxY = 0,minX = 2000,minY = 2000;
	//get the rect points
	for(int i=0;i<4;i++){
		x[i] = rect[i].x;
		y[i] = rect[i].y;
		if(maxX<x[i])
			maxX = x[i];
		if(maxY<y[i])
			maxY = y[i];
		if(minX>x[i])
			minX = x[i];
		if(minY>y[i])
			minY = y[i];
	}
	rectInfo[0] = minY;
	rectInfo[1] = minX;
	rectInfo[2] = maxY - minY;
	rectInfo[3] = maxX - minX;
	return rectInfo;
}
void CVFunction::find()
{
	//1.对输入灰度图片进行高斯滤波
	cv::Mat src = cv::imread("F:\\t13.bmp",CV_BGR2GRAY);
	cv::Mat hsv;
	GaussianBlur(src,hsv,cv::Size(5,5),0,0);

	//2.做灰度直方图，提取阈值，做二值化处理
	// Quantize the gray scale to 30 levels
	int gbins = 16;
	int histSize[] = {gbins};
	   // gray scale varies from 0 to 256
	float granges[] = {0,256};
	const float* ranges[] = { granges };
	cv::MatND hist;
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = {0};

	//calculate hist
	calcHist( &hsv, 1, channels, cv::Mat(), // do not use mask
				hist, 1, histSize, ranges,
				true, // the histogram is uniform
				false );
	//find the max value of hist
	double maxVal=0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);

	int scale = 20;
	cv::Mat histImg;
	histImg.create(500,gbins*scale,CV_8UC3);

	//show gray scale of hist image
	for(int g=0;g<gbins;g++){
		float binVal = hist.at<float>(g,0);
		int intensity = cvRound(binVal*255);
		rectangle( histImg, cv::Point(g*scale,0),
						   cv::Point((g+1)*scale - 1,binVal/maxVal*400),
							CV_RGB(0,0,0),
						   CV_FILLED );
	}
	cv::imshow("histImg",histImg);

	//threshold processing
	cv::Mat hsvRe;
	threshold( hsv, hsvRe, 64, 255,cv::THRESH_BINARY);

	//3.提取图片轮廓
	//4.识别矩形
	vector<Point> approx;
	CvSeq* contours;
	// 找到所有轮廓并且存储在序列中

}



std::vector<std::vector<cv::Point> > CVFunction::findSquaresInImage(cv::Mat _image)
{
	std::vector<std::vector<cv::Point> > squares;
	cv::Mat pyr, timg, gray0(_image.size(), CV_8U), gray;
	int thresh = 50, N = 11;
	cv::pyrDown(_image, pyr, cv::Size(_image.cols/2, _image.rows/2));
	cv::pyrUp(pyr, timg, _image.size());
	std::vector<std::vector<cv::Point> > contours;
	for( int c = 0; c < 3; c++ ) {
		int ch[] = {c, 0};
		mixChannels(&timg, 1, &gray0, 1, ch, 1);
		for( int l = 0; l < N; l++ ) {
			if( l == 0 ) {
				cv::Canny(gray0, gray, 0, thresh, 5);
				cv::dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
			}
			else {
				gray = gray0 >= (l+1)*255/N;
			}
			cv::findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
			std::vector<cv::Point> approx;
			for( size_t i = 0; i < contours.size(); i++ )
			{
				cv::approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
				if( approx.size() == 4 && fabs(contourArea(cv::Mat(approx))) > 1000 && cv::isContourConvex(cv::Mat(approx))) {
					double maxCosine = 0;

					for( int j = 2; j < 5; j++ )
					{
						double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
						maxCosine = MAX(maxCosine, cosine);
					}

					if( maxCosine < 0.3 ) {
						squares.push_back(approx);
					}
				}
			}
		}
	}
	return squares;
}



/*

Mat num[15];
Mat sample;
void deal(Mat &src,int order);
double compare(Mat &src, Mat &sample);
void Threshold(Mat &src,Mat &sample,int m);

void Threshold(Mat &src,Mat &sample ,int m)
{
	cvtColor(sample, sample, COLOR_BGR2GRAY);
	threshold(sample, sample, 48, 255, CV_THRESH_BINARY_INV);
	result123[m].bi = compare(src,sample);
	result123[m].num = m;
}

void deal(Mat &src,int order)
{
	sample = imread("/rong/tmp/text/0.jpg");
	Threshold(src,sample,0);

	sample = imread("/rong/tmp/text/1.jpg");
	Threshold(src,sample,1);

	sample = imread("/rong/tmp/text/2.jpg");
	Threshold(src,sample,2);

	sample = imread("/rong/tmp/text/3.jpg");
	Threshold(src,sample,3);

	sample = imread("/rong/tmp/text/4.jpg");
	Threshold(src,sample,4);

	sample = imread("/rong/tmp/text/5.jpg");
	Threshold(src,sample,5);

	sample = imread("/rong/tmp/text/6.jpg");
	Threshold(src,sample,6);

	sample = imread("/rong/tmp/text/7.jpg");
	Threshold(src,sample,7);

	sample = imread("/rong/tmp/text/8.jpg");
	Threshold(src,sample,8);

	sample = imread("/rong/tmp/text/9.jpg");
	Threshold(src,sample,9);

	sort(result123,result123+10,bba);

	if(result123[9].bi > 0.6) {
		cout << "第" << order << "个数字为 "<< result123[9]. num<<endl;
		cout << "识别精度为 " << result123[9].bi <<endl;
	}
	else cout << "第" << order << "个数字无法识别"<<endl;
}

double compare(Mat &src, Mat &sample)
{
	double same = 0.0, difPoint = 0.0;
	Mat now;
	resize(sample,now,src.size());
	int row = now.rows;
	int col = now.cols *  now.channels();
	for(int i = 0; i < 1; i++){
		uchar * data1 = src.ptr<uchar>(i);
		uchar * data2 = now.ptr<uchar>(i);
		for(int j = 0; j < row * col; j++){
			int  a = data1[j];
			int b = data2[j];
			if( a == b)same++;
			else difPoint++;
		}
	}
	return same/(same+difPoint) ;
}


int CVFunction::find_text( )
{
	Mat srcImage = imread("/rong/tmp/text/number.png");
	Mat dstImage, grayImage, Image;
	srcImage.copyTo(dstImage);
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	threshold(grayImage, Image, 48, 255, CV_THRESH_BINARY_INV);

	//定义轮廓和层次结构
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(Image,contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	int i = 0;
	Point2f pp[5][4];
	vector<vector<Point>>::iterator It;
	Rect rect[10];
	for(It = contours.begin();It < contours.end();It++){                        //画出可包围数字的最小矩形
		Point2f vertex[4];
		rect[i] = boundingRect(*It);
		vertex[0] = rect[i].tl();                                                           //矩阵左上角的点
		vertex[1].x = (float)rect[i].tl().x, vertex[1].y = (float)rect[i].br().y;           //矩阵左下方的点
		vertex[2] = rect[i].br();                                                           //矩阵右下角的点
		vertex[3].x = (float)rect[i].br().x, vertex[3].y = (float)rect[i].tl().y;           //矩阵右上方的点

		for( int j = 0; j < 4; j++)
			line(dstImage,vertex[j], vertex[ (j+1)%4 ],Scalar(0,0,255),1);

		con[i].x = (vertex[0].x+vertex[1].x+vertex[2].x+vertex[3].x) / 4.0;                  //根据中心点判断图像的位置
		con[i].y = (vertex[0].y+vertex[1].y+vertex[2].y+vertex[3].y) / 4.0;
		con[i].order = i;
		i++;
	}
	sort(con,con+i,bbc);

	for(int j = 0; j < i; j++){
		int k = con[j].order;
		srcImage(rect[k]).copyTo(num[j]);
		cvtColor(num[j], num[j], COLOR_BGR2GRAY);
		threshold(num[j], num[j], 48, 255, CV_THRESH_BINARY_INV);
		deal(num[j],j+1);
	}

	system("pause");
	return 0;
}
*/

//Image Stitching

Mat crop_black_borders(Mat img)
{
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);
	Mat binary_img;
	threshold(gray, binary_img, 1, 255, CV_THRESH_BINARY);

	std::vector<std::vector<Point> > contours;
	findContours(binary_img, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	std::vector<std::vector<Point> > contours_poly(contours.size());
	approxPolyDP(Mat(contours[0]), contours_poly[0], 3, true);
	Rect boundRect = boundingRect(Mat(contours[0]));
	Mat out = img(boundRect);

	return out;
}


Mat Image_Stitching( QString file_name1, QString file_name2)
{
	qDebug() << "start work.....";
	Mat img_1 = imread(file_name1.toStdString());
	Mat img_2 = imread(file_name2.toStdString());

	// Detecting keypoints and compute descriptors
	Ptr<ORB> orb = ORB::create();
	std::vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	orb->detectAndCompute(img_1, Mat(), keypoints1, descriptors1);
	orb->detectAndCompute(img_2, Mat(), keypoints2, descriptors2);

	// Matching descriptors
	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce-Hamming");
	std::vector<DMatch> matches;
	descriptorMatcher->match(descriptors1, descriptors2, matches, Mat());

	// Drawing the results
	// Keep best matches only to have a nice drawing.
	// We sort distance between descriptor matches
	Mat index;
	int nbMatch = int(matches.size());
	Mat tab(nbMatch, 1, CV_32F);
	for (int i = 0; i<nbMatch; i++)
	{
		tab.at<float>(i, 0) = matches[i].distance;
	}
	sortIdx(tab, index, SORT_EVERY_COLUMN + SORT_ASCENDING);
	std::vector<DMatch> bestMatches;
	for (int i = 0; i < 200; i++)
	{
		bestMatches.push_back(matches[index.at<int>(i, 0)]);
	}
	Mat matching_result;
	drawMatches(img_1, keypoints1, img_2, keypoints2, bestMatches, matching_result);


	// Localize the object
	std::vector<Point2f> img_1_points;
	std::vector<Point2f> img_2_points;
	for (size_t i = 0; i < bestMatches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		img_1_points.push_back(keypoints1[bestMatches[i].queryIdx].pt);
		img_2_points.push_back(keypoints2[bestMatches[i].trainIdx].pt);
	}
	Mat H = findHomography(img_1_points, img_2_points, RANSAC);

	Mat stitching_result;
	warpPerspective(img_1, stitching_result, H, Size(img_1.cols + img_2.cols, img_1.rows));
    int col = qMin(stitching_result.cols, img_2.cols);
    int row = qMin(stitching_result.rows, img_2.rows);
    Mat half(stitching_result, Rect(0, 0, col, row));
	img_2.copyTo(half);

	Mat cropped_stitching_result = crop_black_borders(stitching_result);


	//namedWindow("Result", 0);
	//imshow("Result", cropped_stitching_result);

	//waitKey(0);

	return cropped_stitching_result;
}

Mat Advanced_Edge_Detection(QString file_name, QString dir_name)
{
	Mat img = imread(file_name.toStdString());
	Mat blur, imgLaplacian, imgCanny;
	vector<Mat> rgbColourEdges(3);

	// Smooth the image to reduce noise for more accurate edge detection
	GaussianBlur(img.clone(), blur, Size(3, 3), 0, 0, BORDER_DEFAULT);

	// Now for colour edge detection
	vector<Mat> channels(3);
	split(blur.clone(), channels);
	double T = 20;
	double maxval = 255;
	for (int i = 0; i < 3; i++)
	{
		// Compute the second-order derivate i.e Laplacian operator
		Laplacian(blur.clone(), rgbColourEdges[i], CV_32F);
		convertScaleAbs(rgbColourEdges[i], rgbColourEdges[i]);

		threshold(rgbColourEdges[i], rgbColourEdges[i], T, maxval, THRESH_BINARY);

	}

	// Logical operation to between colour channels
	Mat tempEdges;
	cv::bitwise_or(rgbColourEdges[0], rgbColourEdges[1], tempEdges);
	cv::bitwise_or(tempEdges, rgbColourEdges[2], imgLaplacian);

	// Convert pixel type for display
	imgLaplacian.convertTo(imgLaplacian, CV_8U);

	// Apply the Canny Edge Detection algorithm
	Canny(img.clone(), imgCanny, 200, 230);


	imwrite( dir_name.toStdString()+ "s_orig.png",img);
	imwrite( dir_name.toStdString() + "laplacian.png",imgLaplacian);
	imwrite( dir_name.toStdString() + "canny.png",imgCanny);
	//imshow("Original Image", img);
	//imshow("Laplacian Edges", imgLaplacian);
	//imshow("Canny Edges", imgCanny);

	//imwrite("Laplacian_T40.jpg", imgLaplacian);
	//imwrite("Canny_T1_200_T2_230.jpg", imgCanny);

	//waitKey(0);
	return imgCanny;
}


int findBiggestContour(std::vector<std::vector<Point> > contours)
{
	int indexOfBiggestContour = -1;
	int sizeOfBiggestContour = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > sizeOfBiggestContour)
		{
			sizeOfBiggestContour = contours[i].size();
			indexOfBiggestContour = i;
		}
	}
	return indexOfBiggestContour;
}

Mat Colour_Segmentation(QString file_name)
{
	Mat img = imread(file_name.toStdString());

	// First smooth the image
	GaussianBlur(img.clone(), img, Size(5, 5), 0.67, 0.67);

	// Convert the image to the HSV colour space for segmentation processing
	Mat hsv;
	cvtColor(img, hsv, COLOR_BGR2HSV);

	// Segment the image based on the specified colour range
	Mat segmented_img;
	inRange(hsv, Scalar(0, 10, 60), Scalar(20, 150, 255), segmented_img);

	// ******OPTIONAL*******
	// Select only the biggest item within the colour range
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;

	findContours(segmented_img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	int s = findBiggestContour(contours);

	Mat drawing = Mat::zeros(img.size(), CV_8UC1);
	drawContours(drawing, contours, s, Scalar(255), -1, 8, hierarchy, 0, Point());

	// Create a structuring element
	int dialation_size = 6;
	Mat element = getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * dialation_size + 1, 2 * dialation_size + 1),
		cv::Point(dialation_size, dialation_size));

	// Apply dilation on the image
	dilate(drawing.clone(), drawing, element);  // dilate(image,dst,element);

	return drawing;
}


QList<Mat> Colour_Transfer(QStringList file_lt)
{
	if(file_lt.count( ) != 2)
	{
		qDebug() << "Please input two images";

		return QList<Mat>();
	}
	qDebug() << "Colour_Transfer";
	//Load the images
	Mat style_img = imread(file_lt[0].toStdString());//("paint.jpg");
	Mat tar_img = imread(file_lt[1].toStdString());//("clock_tower.jpg");

	// Convert to Lab space and CV_32F1
	Mat style_lab, tar_lab;
	cvtColor(style_img, style_lab, COLOR_BGR2Lab);
	cvtColor(tar_img, tar_lab, COLOR_BGR2Lab);
	style_lab.convertTo(style_lab, CV_32FC1);
	tar_lab.convertTo(tar_lab, CV_32FC1);

	//Find mean and std of each channel for each image
	Mat mean_style, mean_tar, std_dev_style, std_dev_tar;
	meanStdDev(style_lab, mean_style, std_dev_style);
	meanStdDev(tar_lab, mean_tar, std_dev_tar);

	// Split into individual channels
	std::vector<Mat> style_chs, tar_chs;
	split(style_lab, style_chs);
	split(tar_lab, tar_chs);
qDebug() << "Colour_Transfer 1";
	// For each channel calculate the color distribution
	for (int i = 0; i < 3; i++)
	{
		// Normalize the colour channels by subtracting the mean a dividing by the standard deviation
		tar_chs[i] -= mean_tar.at<double>(i);
		tar_chs[i] /= std_dev_tar.at<double>(i);

		// Apply the colour style of the style image by reversing the operation,
		// multiplying by the standard deviation and adding the mean of the style image
		tar_chs[i] *= std_dev_style.at<double>(i);
		tar_chs[i] += mean_style.at<double>(i);
	}
qDebug() << "Colour_Transfer 2";
	// Merge the channels, convert to CV_8UC1 each channel and convert to BGR
	Mat out;
	merge(tar_chs, out);
	out.convertTo(out, CV_8UC1);
	cvtColor(out, out, COLOR_Lab2BGR);

	// Show images
	//namedWindow("Style image", 0);
	//imshow("Style image", style_img);
	//namedWindow("Target image", 0);
	//imshow("Target image", tar_img);
	//namedWindow("Output image", 0);
	//imshow("Output image", out);

	//waitKey(0);
qDebug() << "Colour_Transfer 3";
	return QList<Mat>() << style_img << tar_img<<out;
}


Mat Contrast_Enhancement(QString file_name)
{
	Mat img = imread(file_name.toStdString(), IMREAD_UNCHANGED); // Read in the image file


	// Contrast Stretch

	// First build a LUT
	unsigned char lut[256];
	double minVal, maxVal;
	minMaxLoc(img, &minVal, &maxVal); //find minimum and maximum intensities
	for (int i = 0; i < 256; i++)
	{
		lut[i] = 255*((i/(maxVal - minVal)) - (minVal / (maxVal - minVal)));
	}

	// Convert the image type to YCrCb
	Mat YCrCb;
	cvtColor(img, YCrCb, CV_BGR2YCrCb);
	vector<Mat> channels;
	split(YCrCb, channels);

	// Establish the number of bins
	int histSize = 256;

	// Set the range
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat hist;

	// Compute the histogram
	calcHist(&channels[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histogram
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	// Normalize the result to [ 0, histImage.rows ]
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	// Display and Save
	//namedWindow("Original Image Histogram", CV_WINDOW_AUTOSIZE);
	//imshow("Original Image Histogram", histImage);
	imwrite("Original_Image_Histogram.jpg", histImage);

	// Now stretch the contrast
	for (int i = 0; i < channels[0].rows; i++)
	{
		// Create a pointer to the pixel array
		uchar *p = channels[0].ptr<uchar>(i);
		for (int j = 0; j < channels[0].cols; j++)
		{
			p[j] = lut[p[j]];
		}
	}

	// Compute the histogram
	calcHist(&channels[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

	Mat histEnhanced(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	// Normalize the result to [ 0, histImage.rows ]
	normalize(hist, hist, 0, histEnhanced.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(histEnhanced, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	// Display and Save
	//namedWindow("Enhance Image Histogram", CV_WINDOW_AUTOSIZE);
	//imshow("Enhance Image Histogram", histEnhanced);
	imwrite("Enhanced_Image_Histogram.jpg", histEnhanced);

	merge(channels, YCrCb);
	cvtColor(YCrCb, img, CV_YCrCb2BGR);

	//namedWindow("Image Window", WINDOW_AUTOSIZE); // Create a window of the same size as the image for display
	//imshow("Image Window", img); // Show our image inside the window
	//imwrite("image.jpg", img);
	//waitKey(0); // Wait for a keystroke in the window
	return img;
}


Mat Edge_Detection(QString file_name)
{
	Mat img = imread(file_name.toStdString(), CV_LOAD_IMAGE_UNCHANGED);
	Mat blur, gray, GrayEdges, finalColourEdges;
	vector<Mat> rgbColourEdges(3);

	// Smooth the image to reduce noise for more accurate edge detection
	GaussianBlur(img.clone(), blur, Size(3, 3), 0, 0, BORDER_DEFAULT);

	// Convert it to grayscale
	cvtColor(blur.clone(), gray, CV_BGR2GRAY);

	// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	// Gradient X
	//Scharr(gray, grad_x, CV_32F, 1, 0, 1, 0, BORDER_DEFAULT);
	Sobel(gray, grad_x, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	// Gradient Y
	//Scharr(gray, grad_y, CV_32F, 0, 1, 1, 0, BORDER_DEFAULT);
	Sobel(gray, grad_y, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	// Calculate the magnitude
	Mat temp, tempFloat;
	pow(grad_x, 2, grad_x);
	pow(grad_y, 2, grad_y);
	temp = grad_x + grad_y;

	temp.convertTo(tempFloat, CV_32F); // We need CV_32F for the sqrt() function
	sqrt(temp, GrayEdges);

	GrayEdges.convertTo(GrayEdges, CV_8U); // Convert back for display

	double T = 120;
	double maxval = 255;
	threshold(GrayEdges, GrayEdges, T, maxval, THRESH_BINARY);

	//imshow("Original Image", img);
	//imshow("Grayscale Edge Detection", GrayEdges);
	imwrite("Scharr_GrayEdges_thresh120.jpg", GrayEdges);

	// Now for colour edge detection
	vector<Mat> channels(3);
	split(blur.clone(), channels);

	for (int i = 0; i < 3; i++)
	{
		// Generate grad_x and grad_y
		Mat colour_grad_x, colour_grad_y;
		Mat colour_abs_grad_x, colour_abs_grad_y;

		// Gradient X
		//Scharr(channels[i], colour_grad_x, CV_32F, 1, 0, 1, 0, BORDER_DEFAULT);
		Sobel(channels[i], colour_grad_x, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT);
		convertScaleAbs(colour_grad_x, colour_abs_grad_x);

		// Gradient Y
		//Scharr(channels[i], colour_grad_y, CV_32F, 0, 1, 1, 0, BORDER_DEFAULT);
		Sobel(channels[i], colour_grad_y, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT);
		convertScaleAbs(colour_grad_y, colour_abs_grad_y);

		// Calculate the magnitude
		Mat colour_temp, colour_tempFloat;
		pow(colour_grad_x, 2, colour_grad_x);
		pow(colour_grad_y, 2, colour_grad_y);
		colour_temp = colour_grad_x + colour_grad_y;

		colour_temp.convertTo(colour_tempFloat, CV_32F); // We need CV_32F for the sqrt() function
		sqrt(colour_temp, rgbColourEdges[i]);

		threshold(rgbColourEdges[i], rgbColourEdges[i], T, maxval, THRESH_BINARY);
	}

	Mat checkEdges;
	bitwise_or(rgbColourEdges[0], rgbColourEdges[1], checkEdges);
	bitwise_or(checkEdges, rgbColourEdges[2], finalColourEdges);

	finalColourEdges.convertTo(finalColourEdges, CV_8U);

	//imshow("Colour Edge Detection", finalColourEdges);
	//imwrite("Scharr_ColourEdges_thresh120_OR.jpg", finalColourEdges);

	//waitKey(0);
	return finalColourEdges;
}

//Harris Corner Detector--
Mat Harris_Corner_Detector(QString f_name)
{
	// Input image
	Mat img = imread(f_name.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img, CV_32F); // 32-bit float type

	float cornerThreshold = 0.02; // Cornerness threshold value

	// (1) First we compute the derivatives
	Mat grad_x, grad_y;
	Sobel(img.clone(), grad_x, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	Sobel(img.clone(), grad_y, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT);

	// (2) We need to find the matrix M which is defined as:
	// [X*X, X*Y; X*Y, Y*Y]
	// Where X and Y are the horizontal and vertical derivatives of the image, respectively
	Mat XX = Mat::zeros(Size(img.cols, img.rows), CV_32F),
		XY = Mat::zeros(Size(img.cols, img.rows), CV_32F),
		YY = Mat::zeros(Size(img.cols, img.rows), CV_32F);
	multiply(grad_x.clone(), grad_x.clone(), XX);
	multiply(grad_x.clone(), grad_y.clone(), XY);
	multiply(grad_y.clone(), grad_y.clone(), YY);

	// (3) Now we apply the Gaussian filter to have spatial weighting
	GaussianBlur(XX.clone(), XX, Size(7, 7), 1, 1);
	GaussianBlur(XY.clone(), XY, Size(7, 7), 1, 1);
	GaussianBlur(YY.clone(), YY, Size(7, 7), 1, 1);

	// (4) and(5) Now we build the matrix M, compute the cornerness R, and threshold it
	float alpha = 0.05;
	Mat M = Mat::zeros(Size(2, 2), CV_32F);
	vector<float> eigenvalues(2);
	Mat cornerness = Mat::zeros(Size(img.cols, img.rows), CV_32F);
	for (int i = 0; i < img.rows; i++)
	{
		// Create a pointer to the pixel array
		float *xx = XX.ptr<float>(i);
		float *xy = XY.ptr<float>(i);
		float *yy = YY.ptr<float>(i);
		for (int j = 0; j < img.cols; j++)
		{
			M.at<float>(0, 0) = xx[j];
			M.at<float>(0, 1) = xy[j];
			M.at<float>(1, 0) = xy[j];
			M.at<float>(1, 1) = yy[j];

			eigen(M, eigenvalues);

			cornerness.at<float>(i, j) = (float)determinant(M) - alpha*((float)trace(M)[0] * (float)trace(M)[0]);
			//cornerness.at<float>(i, j) = eigenvalues[0] * eigenvalues[1] - alpha*(eigenvalues[0] + eigenvalues[1])*(eigenvalues[0] + eigenvalues[1]);

		}
	}

	// Threshold
	threshold(cornerness, cornerness, 100000000, 255, THRESH_TOZERO);

	// Non-Maximum Suppression
	int rad = 3;
	int dim = 2 * rad + 1;

	for (int i = rad; i < img.rows - rad; i++) // Start looping slightly inward to handle the borders for sliding window
	{
		// Create a pointer to the pixel array
		float *p = cornerness.ptr<float>(i);

		for (int j = rad; j < img.cols - rad; j++)
		{
			Mat NMS = cornerness(Range(j - rad, j + rad), Range(i - rad, i + rad)); // Extract the sliding window
			double minVal, maxVal;
			minMaxLoc(NMS, &minVal, &maxVal);
			if (p[j] < maxVal)
			{
				p[j] = 0;
			}
		}
	}

	// Draw the corner points on the image
	for (int i = 0; i < img.rows; i++)
	{
		float *p = cornerness.ptr<float>(i);
		for (int j = 0; j < img.cols; j++)
		{
			if (p[j] > 0) { circle(img, Point(j, i), 1, Scalar(0, 0, 255)); }
		}
	}

	// Convert to uchar for display
	img.convertTo(img, CV_8U);
	//imshow("Corners Image", img);

	//waitKey(0);
	return img;
}


//Histogram Equalization
Mat Histogram_Equalization(QString f_name)
{
	Mat img = imread(f_name.toStdString(), IMREAD_GRAYSCALE); // Read in the image file

	if (img.empty()){ "\nImage is empty!!\n"; }

	// Establish the number of bins
	int histSize = 256;

	// Set the range
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat hist;

	// Compute the histogram
	calcHist(&img, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histogram
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	// Normalize the result to [ 0, histImage.rows ]
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	// Display and Save
	//namedWindow("Original Image Histogram", CV_WINDOW_AUTOSIZE);
	//imshow("Original Image Histogram", histImage);
	imwrite("Original_Image_Histogram.jpg", histImage);

	//namedWindow("Image Window", WINDOW_AUTOSIZE); // Create a window of the same size as the image for display
	//imshow("Image Window", img); // Show our image inside the window
	imwrite("image.jpg", img);

	// Perform Global Histogram Equalization
	Mat out;
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(2.0);
	clahe->apply(img, out);
	//equalizeHist(img, out);

	// Compute the histogram
	calcHist(&out, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

	Mat outHistImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	// Normalize the result to [ 0, histImage.rows ]
	normalize(hist, hist, 0, outHistImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(outHistImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	// Display and Save
	//namedWindow("Equalized Image Histogram", CV_WINDOW_AUTOSIZE);
	//imshow("Equalized Image Histogram", outHistImage);
	imwrite("Equalized_Image_Histogram.jpg", outHistImage);

	//namedWindow("Equalized Image Window", WINDOW_AUTOSIZE); // Create a window of the same size as the image for display
	//imshow("Equalized Image Window", out); // Show our image inside the window
	imwrite("Equalized_Image.jpg", out);

	//waitKey(0); // Wait for a keystroke in the window
	return out;
}

//Image Sharpening
Mat Image_Sharpening(QString f_name)
{
	Mat img = imread(f_name.toStdString());
	Mat laplacianSharpening, unsharpSharpening;

	// Laplacian sharpening
	float laplacianBoostFactor = 1.2; // Controls the amount of Laplacian Sharpening
	Mat kern = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5*laplacianBoostFactor, -1, 0, -1, 0); // The filtering mask for Laplacian sharpening
	filter2D(img.clone(), laplacianSharpening, img.depth(), kern, Point(-1, -1)); // Applies the masking operator to the image

	// Unsharp mask sharpening
	Mat blur;
	GaussianBlur(img.clone(), blur, Size(5, 5), 0.67, 0.67);
	Mat unsharpMask = img.clone() - blur; // Compute the unsharp mask
	threshold(unsharpMask.clone(), unsharpMask, 20, 255, THRESH_TOZERO);
	float unsharpBoostFactor = 9; // Controls the amount of Unsharp Mask Sharpening
	unsharpSharpening = img.clone() + (unsharpMask * unsharpBoostFactor);

	//imshow("Original Image", img);
	//imshow("Laplacian Sharpened Image", laplacianSharpening);
	//imshow("Unsharp Masking Sharpening", unsharpSharpening);

	imwrite("Laplacian_Sharp_Boost_1_point_2.jpg", laplacianSharpening);
	imwrite("Unsharp_Sharp_Boost_9_Threshold_20.jpg", unsharpSharpening);

	//waitKey(0);
	return img;
}

//Image Smoothing
Mat Image_Smoothing(QStringList f_lt)
{
	if(f_lt.count( ) != 2)
	{
		qDebug() << "Please input two images";

		return Mat();
	}
	Mat inputGaussian = imread(f_lt[0].toStdString(), CV_LOAD_IMAGE_UNCHANGED);
	Mat inputImpulse = imread(f_lt[1].toStdString(), CV_LOAD_IMAGE_UNCHANGED);

	Mat imgBox3x3, imgBox7x7, imgGauss3x3, imgGauss7x7, imgMedian3x3, imgMedian7x7;

	/*namedWindow("Gaussian Noise Image", 1);
	namedWindow("Impulse Noise Image", 1);
	imshow("Gaussian Noise Image", inputGaussian);
	imshow("Impulse Noise Image", inputImpulse);
*/
	boxFilter(inputGaussian.clone(), imgBox3x3, inputGaussian.depth(), Size(3, 3));
	boxFilter(inputGaussian.clone(), imgBox7x7, inputGaussian.depth(), Size(7, 7));

	int wndSize = 3;
	double sigma = (wndSize - 1) / 6;
	GaussianBlur(inputGaussian.clone(), imgGauss3x3, Size(wndSize, wndSize), sigma, sigma);
	wndSize = 7;
	sigma = (wndSize - 1) / 6;
	GaussianBlur(inputGaussian.clone(), imgGauss7x7, Size(wndSize, wndSize), sigma, sigma);

	medianBlur(inputGaussian.clone(), imgMedian3x3, 3);
	medianBlur(inputGaussian.clone(), imgMedian7x7, 7);

	//namedWindow("Gaussian Noise, 3x3 Box Blurred Image", 1);
	//namedWindow("Gaussian Noise, 3x3 Gaussian Blurred Image", 1);
	//namedWindow("Gaussian Noise, 3x3 Median Blurred Image", 1);
	//namedWindow("Gaussian Noise, 7x7 Box Blurred Image", 1);
	//namedWindow("Gaussian Noise, 7x7 Gaussian Blurred Image", 1);
	//namedWindow("Gaussian Noise, 7x7 Median Blurred Image", 1);

	//imshow("Gaussian Noise, 3x3 Box Blurred Image", imgBox3x3);
	//imshow("Gaussian Noise, 3x3 Gaussian Blurred Image", imgGauss3x3);
	//imshow("Gaussian Noise, 3x3 Median Blurred Image", imgMedian3x3);
	//imshow("Gaussian Noise, 7x7 Box Blurred Image", imgBox7x7);
	//imshow("Gaussian Noise, 7x7 Gaussian Blurred Image", imgGauss7x7);
	//imshow("Gaussian Noise, 7x7 Median Blurred Image", imgMedian7x7);

	imwrite("gaussian_noise_imgBox3x3.jpg", imgBox3x3);
	imwrite("gaussian_noise_imgBox7x7.jpg", imgBox7x7);
	imwrite("gaussian_noise_imgGauss3x3.jpg", imgGauss3x3);
	imwrite("gaussian_noise_imgGauss7x7.jpg", imgGauss7x7);
	imwrite("gaussian_noise_imgMedian3x3.jpg", imgMedian3x3);
	imwrite("gaussian_noise_imgMedian7x7.jpg", imgMedian7x7);

	boxFilter(inputImpulse.clone(), imgBox3x3, inputImpulse.depth(), Size(3, 3));
	boxFilter(inputImpulse.clone(), imgBox7x7, inputImpulse.depth(), Size(7, 7));

	wndSize = 3;
	sigma = (wndSize - 1) / 6;
	GaussianBlur(inputImpulse.clone(), imgGauss3x3, Size(wndSize, wndSize), sigma, sigma);
	wndSize = 7;
	sigma = (wndSize - 1) / 6;
	GaussianBlur(inputImpulse.clone(), imgGauss7x7, Size(wndSize, wndSize), sigma, sigma);

	medianBlur(inputImpulse.clone(), imgMedian3x3, 3);
	medianBlur(inputImpulse.clone(), imgMedian7x7, 7);
/*
	namedWindow("Impulse Noise, 3x3 Box Blurred Image", 1);
	namedWindow("Impulse Noise, 3x3 Gaussian Blurred Image", 1);
	namedWindow("Impulse Noise, 3x3 Median Blurred Image", 1);
	namedWindow("Impulse Noise, 7x7 Box Blurred Image", 1);
	namedWindow("Impulse Noise, 7x7 Gaussian Blurred Image", 1);
	namedWindow("Impulse Noise, 7x7 Median Blurred Image", 1);

	imshow("Impulse Noise, 3x3 Box Blurred Image", imgBox3x3);
	imshow("Impulse Noise, 3x3 Gaussian Blurred Image", imgGauss3x3);
	imshow("Impulse Noise, 3x3 Median Blurred Image", imgMedian3x3);
	imshow("Impulse Noise, 7x7 Box Blurred Image", imgBox7x7);
	imshow("Impulse Noise, 7x7 Gaussian Blurred Image", imgGauss7x7);
	imshow("Impulse Noise, 7x7 Median Blurred Image", imgMedian7x7);
*/
	imwrite("Impulse_noise_imgBox3x3.jpg", imgBox3x3);
	imwrite("Impulse_noise_imgBox7x7.jpg", imgBox7x7);
	imwrite("Impulse_noise_imgGauss3x3.jpg", imgGauss3x3);
	imwrite("Impulse_noise_imgGauss7x7.jpg", imgGauss7x7);
	imwrite("Impulse_noise_imgMedian3x3.jpg", imgMedian3x3);
	imwrite("Impulse_noise_imgMedian7x7.jpg", imgMedian7x7);

	//waitKey(0);

	return imgMedian7x7;
}


//Image Stitching

Mat Image_Stitching(QStringList f_lt)
{
	if(f_lt.count( ) != 2)
	{
		qDebug() << "Please input two images";

		return Mat();
	}
	Mat img_1 = imread(f_lt[0].toStdString());
	Mat img_2 = imread(f_lt[1].toStdString());

	// Detecting keypoints and compute descriptors
	Ptr<ORB> orb = ORB::create();
	std::vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	orb->detectAndCompute(img_1, Mat(), keypoints1, descriptors1);
	orb->detectAndCompute(img_2, Mat(), keypoints2, descriptors2);

	// Matching descriptors
	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce-Hamming");
	std::vector<DMatch> matches;
	descriptorMatcher->match(descriptors1, descriptors2, matches, Mat());

	// Drawing the results
	// Keep best matches only to have a nice drawing.
	// We sort distance between descriptor matches
	Mat index;
	int nbMatch = int(matches.size());
	Mat tab(nbMatch, 1, CV_32F);
	for (int i = 0; i<nbMatch; i++)
	{
		tab.at<float>(i, 0) = matches[i].distance;
	}
	sortIdx(tab, index, SORT_EVERY_COLUMN + SORT_ASCENDING);
	std::vector<DMatch> bestMatches;
	for (int i = 0; i < 200; i++)
	{
		bestMatches.push_back(matches[index.at<int>(i, 0)]);
	}
	Mat matching_result;
	drawMatches(img_1, keypoints1, img_2, keypoints2, bestMatches, matching_result);


	// Localize the object
	std::vector<Point2f> img_1_points;
	std::vector<Point2f> img_2_points;
	for (size_t i = 0; i < bestMatches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		img_1_points.push_back(keypoints1[bestMatches[i].queryIdx].pt);
		img_2_points.push_back(keypoints2[bestMatches[i].trainIdx].pt);
	}
	Mat H = findHomography(img_1_points, img_2_points, RANSAC);

	Mat stitching_result;
	warpPerspective(img_1, stitching_result, H, Size(img_1.cols + img_2.cols, img_1.rows));
	Mat half(stitching_result, Rect(0, 0, img_2.cols, img_2.rows));
	img_2.copyTo(half);

	Mat cropped_stitching_result = crop_black_borders(stitching_result);

	//namedWindow("Result", 0);
	//imshow("Result", cropped_stitching_result);

	//waitKey(0);

	return cropped_stitching_result;
}

//Point Operations on Digital Images
Mat Point_Operations_on_Digital_Images(QString f_n)
{
	Mat img = imread(f_n.toStdString(), IMREAD_UNCHANGED); // Read in the image file
	int ch = img.channels();

	// For Colour Images
	for (int i = 0; i < img.rows; i++)
	{
		// Create a pointer to the pixel array
		uchar *p = img.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			p[j*ch + 0] = 0; // B
			p[j*ch + 1] = 0; // G
			p[j*ch + 2] = p[j*ch + 2]; // R
		}
	}

	/*for (int i = 0; i < 720; i++)
	{
		for (int j = 0; j < 720; j++)
		{
			img.at<Vec3b>(i, j)[0] = 0; // B
			img.at<Vec3b>(i, j)[1] = 0; // G
			img.at<Vec3b>(i, j)[2] = img.at<Vec3b>(i, j).val[2]; // R
		}
	}*/

	// For Grayscale Images

	/*
	for (int i = 0; i < img.rows; i++)
	{
		// Create a pointer to the pixel array
		uchar *p = img.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			if (p[j] < 100){ p[j] = 0; }
			else if (p[j] > 155){ p[j] = 255; }
			else { p[j] = p[j]; }
		}
	}*/

	/*float A = 1.5;
	int B = 40;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			img.at<uchar>(i, j) = saturate_cast<uchar>((int)A*img.at<uchar>(i, j) + B);
		}
	}*/

	//namedWindow("Image Window", WINDOW_AUTOSIZE); // Create a window of the same size as the image for display
	//imshow("Image Window", img); // Show our image inside the window
	//imwrite("image.jpg", img);
	//waitKey(0); // Wait for a keystroke in the window
	return img;
}



// Compute the energy matrix of the image
Mat compute_energy_matrix(Mat img)
{

	// For colour edge detection
	std::vector<Mat> channels(3);
	split(img.clone(), channels);
	std::vector<Mat> rgbColourEdges(3);

	for (int i = 0; i < 3; i++)
	{
		// Generate grad_x and grad_y
		Mat colour_grad_x, colour_grad_y;

		// Gradient X
		//Scharr(channels[i], colour_grad_x, CV_32F, 1, 0, 1, 0, BORDER_DEFAULT);
		Sobel(channels[i], colour_grad_x, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT);

		// Gradient Y
		//Scharr(channels[i], colour_grad_y, CV_32F, 0, 1, 1, 0, BORDER_DEFAULT);
		Sobel(channels[i], colour_grad_y, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT);

		rgbColourEdges[i] = abs(colour_grad_x) + abs(colour_grad_y);

		// Convert to float for the energy function
		rgbColourEdges[i].clone().convertTo(rgbColourEdges[i], CV_32F);

	}

	// Compute the final energy function by summing all of the colour channels
	Mat energy_matrix = Mat::zeros(img.size(), CV_32F);
	add(rgbColourEdges[0], rgbColourEdges[1], energy_matrix);
	add(rgbColourEdges[2], energy_matrix.clone(), energy_matrix);

	return energy_matrix;

}

// Compute the score matrix using dynamic programming, given the energy function
Mat compute_score_matrix(Mat energy_matrix)
{
	Mat score_matrix = Mat::zeros(energy_matrix.size(), CV_32F);
	score_matrix.row(0) = energy_matrix.row(0);

	for (int i = 1; i < score_matrix.rows; i++)
	{
		for (int j = 0; j < score_matrix.cols; j++)
		{
			float min_score = 0;

			// Handle the edge cases
			if (j - 1 < 0)
			{
				std::vector<float> scores(2);
				scores[0] = score_matrix.at<float>(i - 1, j);
				scores[1] = score_matrix.at<float>(i - 1, j + 1);
				min_score = *std::min_element(std::begin(scores), std::end(scores));
			}
			else if (j + 1 >= score_matrix.cols)
			{
				std::vector<float> scores(2);
				scores[0] = score_matrix.at<float>(i - 1, j - 1);
				scores[1] = score_matrix.at<float>(i - 1, j);
				min_score = *std::min_element(std::begin(scores), std::end(scores));
			}
			else
			{
				std::vector<float> scores(3);
				scores[0] = score_matrix.at<float>(i - 1, j - 1);
				scores[1] = score_matrix.at<float>(i - 1, j);
				scores[2] = score_matrix.at<float>(i - 1, j + 1);
				min_score = *std::min_element(std::begin(scores), std::end(scores));
			}

			score_matrix.at<float>(i, j) = energy_matrix.at<float>(i, j) + min_score;
		}
	}

	return score_matrix;
}

// Given the score matrix, traverse back up to get the lowest energy seam
std::vector<int> get_seam(Mat score_matrix)
{
	// Get the lowest score in the last row
	std::vector<float> last_row = score_matrix.row(score_matrix.rows - 1);
	int end_idx = std::min_element(std::begin(last_row), std::end(last_row)) - std::begin(last_row);

	// Extract the lowest energy seam
	std::vector<int> low_energy_seam(score_matrix.rows);
	low_energy_seam[low_energy_seam.size() - 1] = end_idx;
	int curr_col_idx = end_idx;
	float inf = std::numeric_limits<float>::infinity();

	for (int i = score_matrix.rows - 1; i > 0; i--)
	{
		int left_col_idx = curr_col_idx - 1;
		int right_col_idx = curr_col_idx + 1;
		int	next_idx = 1;
		std::vector<float> scores(3);

		if (left_col_idx < 0 || left_col_idx >= score_matrix.cols)
		{
			scores = { inf, score_matrix.at<float>(i, curr_col_idx), score_matrix.at<float>(i, right_col_idx) };
		}
		else if (right_col_idx < 0 || right_col_idx >= score_matrix.cols)
		{
			scores = { score_matrix.at<float>(i, left_col_idx), score_matrix.at<float>(i, curr_col_idx), inf };
		}
		else
		{
			scores = { score_matrix.at<float>(i, left_col_idx), score_matrix.at<float>(i, curr_col_idx), score_matrix.at<float>(i, right_col_idx) };
		}

			next_idx = std::min_element(std::begin(scores), std::end(scores)) - std::begin(scores);

			if (next_idx == 0)
			{
				curr_col_idx = left_col_idx;
			}

			if (next_idx == 3)
			{
				curr_col_idx = right_col_idx;
			}

			low_energy_seam[i - 1] = curr_col_idx;
	}


	return low_energy_seam;

}

Mat remove_single_seam(Mat img, std::vector<int> low_energy_seam)
{
	Mat out = Mat::zeros(Size(img.cols - 1, img.rows), img.type());
	int ch = img.channels();

	for (int i = 0; i < img.rows; i++)
	{
		int out_col_idx = 0;

		// Create a pointer to the pixel array
		uchar *img_p = img.ptr<uchar>(i);
		uchar *out_p = out.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			bool keep_pixel = true;

			if (low_energy_seam.at(i) == j)
			{
				keep_pixel = false;
			}

			if (keep_pixel == true)
			{
				out_p[out_col_idx*ch + 0] = img_p[j*ch + 0]; // B
				out_p[out_col_idx*ch + 1] = img_p[j*ch + 1]; // G
				out_p[out_col_idx*ch + 2] = img_p[j*ch + 2]; // R
				out_col_idx += 1;
			}
			else
			{
				continue;
			}
		}
	}

	return out;
}

Mat add_single_seam(Mat img, std::vector<int> low_energy_seam)
{
	Mat out = Mat::zeros(Size(img.cols + 1, img.rows), img.type());
	int ch = img.channels();

	for (int i = 0; i < img.rows; i++)
	{
		int img_col_idx = 0;

		// Create a pointer to the pixel array
		uchar *img_p = img.ptr<uchar>(i);
		uchar *out_p = out.ptr<uchar>(i);
		for (int j = 0; j < img.cols + 1; j++)
		{
			bool new_pixel = false;

			if (low_energy_seam.at(i) == j)
			{
				new_pixel = true;
			}

			if (new_pixel == false)
			{
				out_p[j*ch + 0] = img_p[img_col_idx*ch + 0]; // B
				out_p[j*ch + 1] = img_p[img_col_idx*ch + 1]; // G
				out_p[j*ch + 2] = img_p[img_col_idx*ch + 2]; // R
				img_col_idx += 1;
			}
			else
			{
				// Handle the edge cases
				if (img_col_idx - 1 < 0)
				{
					out_p[j*ch + 0] = (img_p[img_col_idx*ch + 0] + img_p[(img_col_idx + 1)*ch + 0]) / 2; // B
					out_p[j*ch + 1] = (img_p[img_col_idx*ch + 1] + img_p[(img_col_idx + 1)*ch + 1]) / 2; // G
					out_p[j*ch + 2] = (img_p[img_col_idx*ch + 2] + img_p[(img_col_idx + 1)*ch + 2]) / 2; // R
				}
				else if (img_col_idx + 1 >= img.cols)
				{
					out_p[j*ch + 0] = (img_p[(img_col_idx - 1)*ch + 0] + img_p[img_col_idx*ch + 0]) / 2; // B
					out_p[j*ch + 1] = (img_p[(img_col_idx - 1)*ch + 1] + img_p[img_col_idx*ch + 1]) / 2; // G
					out_p[j*ch + 2] = (img_p[(img_col_idx - 1)*ch + 2] + img_p[img_col_idx*ch + 2]) / 2; // R
				}
				else
				{
					out_p[j*ch + 0] = (img_p[(img_col_idx - 1)*ch + 0] + img_p[img_col_idx*ch + 0] + img_p[(img_col_idx + 1)*ch + 0]) / 3; // B
					out_p[j*ch + 1] = (img_p[(img_col_idx - 1)*ch + 1] + img_p[img_col_idx*ch + 1] + img_p[(img_col_idx + 1)*ch + 1]) / 3; // G
					out_p[j*ch + 2] = (img_p[(img_col_idx - 1)*ch + 2] + img_p[img_col_idx*ch + 2] + img_p[(img_col_idx + 1)*ch + 2]) / 3; // R
				}

				img_col_idx = img_col_idx;
			}
		}
	}

	return out;

}

Mat remove_all_seams(Mat img, int num_seams)
{
	for (int i = 1; i <= num_seams; i++)
	{
		Mat energy = compute_energy_matrix(img.clone());
		Mat score = compute_score_matrix(energy);
		std::vector<int> seam = get_seam(score);
		cout << "Removing col seam # " << i << endl;
		img = remove_single_seam(img, seam);
	}

	return img;
}

Mat add_all_seams(Mat img, int num_seams)
{
	// Getting all of the seams to duplicate and add
	// These are the first "num_seams" that we would normally remove
	Mat img_copy = img.clone();

	cout << "Getting all seams " << num_seams << " to add..." << endl;
	std::vector<std::vector<int>> seams_to_add(num_seams);
	for (int i = 0; i < num_seams; i++)
	{
		Mat energy = compute_energy_matrix(img_copy.clone());
		Mat score = compute_score_matrix(energy);
		std::vector<int> seam = get_seam(score);
		seams_to_add.at(i) = seam;
		img_copy = remove_single_seam(img_copy, seam);
		cout << "Got seam # " << i + 1 << endl;
	}

	// Add all of the seams
	for (int i = 0; i < num_seams; i++)
	{
		cout << "Adding seam # " << i+1 << endl;
		img = add_single_seam(img, seams_to_add.at(i));
	}

	return img;

}


Mat seam_carving(Mat img, Size out_size)
{
	int num_row_seams = img.rows - out_size.height;
	int num_col_seams = img.cols - out_size.width;

	if (num_col_seams < 0)
	{
		num_col_seams = std::abs(num_col_seams);
		img = add_all_seams(img, num_col_seams);
	}
	else
	{
		img = remove_all_seams(img, num_col_seams);
	}

	transpose(img, img);

	if (num_row_seams < 0)
	{
		num_row_seams = std::abs(num_row_seams);
		img = add_all_seams(img, num_row_seams);
	}
	else
	{
		img = remove_all_seams(img, num_row_seams);
	}

	transpose(img, img);

	return img;
}

Mat seam_carving(QString f_n)
{
	// Input image
	Mat img = imread(f_n.toStdString());
	Mat img_copy = img.clone();

	if (img.empty())
	{
		cout << "ERROR: Image is empty!!!" << endl;
		return Mat();
	}

	// Prompt the user to input the image size
	cout << "The image is of size = " << img.cols << " x " << img.rows << ". Enter in the new image size." << endl << endl;

	int num_rows = 0;
	int num_cols = 0;
	cout << "rows: "; cin >> num_rows;
	cout << "cols: "; cin >> num_cols;
	cout << endl;

	// Get the number of seams to add or remove

	// Add or remove seams according to the difference in number of rows and cols
	Mat out = seam_carving(img, Size(num_cols, num_rows));

	//namedWindow("Input Image", 0);
	//namedWindow("Output Image", 0);
	//imshow("Input Image", img_copy);
	//imshow("Output Image", out);

	//waitKey(0);

	return out;
}

Mat make_rect_circle(QString f_n)
{
	CVFunction cv;
	cv::Mat src = imread(f_n.toStdString());
	cv::Mat gray;
	cv::Mat bw;
	cv::Mat dst;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Point> approx;

	// Convert to grayscale
	cv::cvtColor(src, gray, CV_BGR2GRAY);

	// Use Canny instead of threshold to catch squares with gradient shading
	blur( gray, bw, Size(3,3) );
	cv::Canny(gray, bw, 80, 240, 3);

	//cv::bitwise_not(bw, bw);

	// Find contours
	cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	src.copyTo(dst);

	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
			continue;

		if (approx.size() == 3)
		{
			cv.setLabel(dst, "TRI", contours[i]);    // Triangles
		}
		else if (approx.size() >= 4 && approx.size() <= 6)
		{
			// Number of vertices of polygonal curve
			int vtc = approx.size();

			// Get the cosines of all corners
			std::vector<double> cos;
			for (int j = 2; j < vtc+1; j++)
				cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

			// Sort ascending the cosine values
			std::sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Use the degrees obtained above and the number of vertices
			// to determine the shape of the contour
			if (vtc == 4 )
				cv.setLabel(dst, "RECT", contours[i]);
			else if (vtc == 5 )
				cv.setLabel(dst, "PENTA", contours[i]);
			else if (vtc == 6 )
				cv.setLabel(dst, "HEXA", contours[i]);
		}
		else
		{
			// Detect and label circles
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = r.width / 2;

			if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
				std::abs(1 - (area / (CV_PI * (radius*radius)))) <= 0.2)
				cv.setLabel(dst, "CIR", contours[i]);
		}
	}
	return dst;
}
