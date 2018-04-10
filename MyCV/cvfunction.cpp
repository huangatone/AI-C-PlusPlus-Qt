#include "cvfunction.h"

#include "colordetector.h"
#include <QtMath>

#include <iostream>
#include <stdio.h>
#include "dfpixmapwdiget.h"
#include "cvfunction.h"
#include <opencv2/tracking.hpp>
#include <QDebug>
#include "colordetector.h"
#include <QMessageBox>
#include "colorhistogram.h"
#include "objectFinder.h"
#include "morphoFeatures.h"

#include "laplacianZC.h"
#include <map>
#include <vector>
#include <QFile>

#include "opencv2/text.hpp"


using namespace cv::text;
using namespace std;



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
	if (img_qt.isNull()){
		//ColorTable for Binary Images
		QVector<QRgb> colorTable;
		for (int i = 0; i < 256; i++)
			colorTable.push_back(qRgb(i, i, i));

		img_qt = QImage((const unsigned char*)result.data,result.cols, result.rows, QImage::Format_Indexed8);
		img_qt.setColorTable(colorTable);
		}

	return img_qt.rgbSwapped();
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

int CVFunction::check()
{
	//cv::Mat src = cv::imread("polygon.png");
	cv::Mat src;
	cv::Mat gray;
	cv::Mat bw;
	cv::Mat dst;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Point> approx;

	VideoCapture capture(0);
	int q;

	while(cvWaitKey(30) != 'q')
	{
		capture >> src;
		if(true)
		{
			// Convert to grayscale
			cv::cvtColor(src, gray, CV_BGR2GRAY);

			// Use Canny instead of threshold to catch squares with gradient shading
			blur( gray, bw, Size(3,3) );
			cv::Canny(gray, bw, 80, 240, 3);
			cv::imshow("bw", bw);
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
			cv::imshow("src", src);
			cv::imshow("dst", dst);

		}
		else
		{
			break;
		}
	}

	return 0;
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
	waitKey(0);
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



double angle123( cv::Point pt1, cv::Point pt2, cv::Point pt0 )
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

std::vector<std::vector<cv::Point> > findSquaresInImage(cv::Mat _image)
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
