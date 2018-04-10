#include "car.h"
#include <QTime>
Car::Car()
{

}


#include <QDebug>


#include <QDir>
#include <QFile>
#include "cvfunction.h"

#define HORIZONTAL    1
#define VERTICAL    0


#include "opencv2/opencv.hpp"
#include "opencv2/ml.hpp"
using namespace cv;




/*////
  1，首先是梯度分布特征，该特征计算图像水平方向和竖直方向的梯度图像，然后通过给梯度图像分划不同的区域，进行梯度图像每个区域亮度值的统计，以下是算法步骤：
<1>将字符由RGB转化为灰度，然后将图像归一化到16*8。

<2>定义soble水平检测算子：x_mask=[−1,0,1;−2,0,2;–1,0,1]和竖直方向梯度检测算子y_mask=x_maskT。

<3>对图像分别用mask_x和mask_y进行图像滤波得到SobelX和SobelY，下图分别代表原图像、SobelX和SobelY。

<4>对滤波后的图像，计算图像总的像素和，然后划分4*2的网络，计算每个网格内的像素值的总和。

<5>将每个网络内总灰度值占整个图像的百分比统计在一起写入一个向量，将两个方向各自得到的向量并在一起，组成特征向量。
/// */

void calcGradientFeat(const Mat& imgSrc, vector<float>& feat)
{
	float sumMatValue(const Mat& image); // 计算图像中像素灰度值总和

	Mat image;
	cvtColor(imgSrc,image,CV_BGR2GRAY);
	resize(image,image,Size(8,16));

	// 计算x方向和y方向上的滤波
	float mask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	Mat y_mask = Mat(3, 3, CV_32F, mask) / 8;
	Mat x_mask = y_mask.t(); // 转置
	Mat sobelX, sobelY;

	filter2D(image, sobelX, CV_32F, x_mask);
	filter2D(image, sobelY, CV_32F, y_mask);

	sobelX = abs(sobelX);
	sobelY = abs(sobelY);

	float totleValueX = sumMatValue(sobelX);
	float totleValueY = sumMatValue(sobelY);

	// 将图像划分为4*2共8个格子，计算每个格子里灰度值总和的百分比
	for (int i = 0; i < image.rows; i = i + 4)
	{
		for (int j = 0; j < image.cols; j = j + 4)
		{
			Mat subImageX = sobelX(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageX) / totleValueX);
			Mat subImageY= sobelY(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageY) / totleValueY);
		}
	}
}
float sumMatValue(const Mat& image)
{
	float sumValue = 0;
	int r = image.rows;
	int c = image.cols;
	if (image.isContinuous())
	{
		c = r*c;
		r = 1;
	}
	for (int i = 0; i < r; i++)
	{
		const uchar* linePtr = image.ptr<uchar>(i);
		for (int j = 0; j < c; j++)
		{
			sumValue += linePtr[j];
		}
	}
	return sumValue;
}

/*
2，第二个特征非常简单，只需要将图像归一化到特定的大小，然后将图像每个点的灰度值作为特征即可。

<1>将图像由RGB图像转换为灰度图像；

<2>将图像归一化大小为8×4，并将图像展开为一行，组成特征向量。
*/


/*
4. OpenCV中的神经网络
关于神经网络的原理我的博客里已经写了两篇文章，并且给出了C++的实现，所以这里我就不提了，下面主要说明在OpenCV中怎么使用它提供的库函数。

CvANN_MLP是OpenCV中提供的一个神经网络的类，正如它的名字一样(multi-layer perceptrons),它是一个多层感知网络，它有一个输入层，一个输出层以及1或多个隐藏层。

4.1. 首先我们来创建一个网络，我们可以利用CvANN_MLP的构造函数或者create函数。*/



#include <fstream>
#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv;

#define SHOW_PROCESS 0
#define ON_STUDY 0

class NumTrainData
{
public:
	NumTrainData()
	{
		memset(data, 0, sizeof(data));
		result = -1;
	}
public:
	float data[64];
	int result;
};

vector<NumTrainData> buffer;
int featureLen = 64;

void swapBuffer(char* buf)
{
	char temp;
	temp = *(buf);
	*buf = *(buf+3);
	*(buf+3) = temp;

	temp = *(buf+1);
	*(buf+1) = *(buf+2);
	*(buf+2) = temp;
}

void GetROI(Mat& src, Mat& dst)
{
	int left, right, top, bottom;
	left = src.cols;
	right = 0;
	top = src.rows;
	bottom = 0;

	//Get valid area
	for(int i=0; i<src.rows; i++)
	{
		for(int j=0; j<src.cols; j++)
		{
			if(src.at<uchar>(i, j) > 0)
			{
				if(j<left) left = j;
				if(j>right) right = j;
				if(i<top) top = i;
				if(i>bottom) bottom = i;
			}
		}
	}

	//Point center;
	//center.x = (left + right) / 2;
	//center.y = (top + bottom) / 2;

	int width = right - left;
	int height = bottom - top;
	int len = (width < height) ? height : width;

	//Create a squre
	dst = Mat::zeros(len, len, CV_8UC1);

	//Copy valid data to squre center
	Rect dstRect((len - width)/2, (len - height)/2, width, height);
	Rect srcRect(left, top, width, height);
	Mat dstROI = dst(dstRect);
	Mat srcROI = src(srcRect);
	srcROI.copyTo(dstROI);
}

int ReadTrainData(int maxCount)
{
	//Open image and label file
	const char fileName[] = "../res/train-images.idx3-ubyte";
	const char labelFileName[] = "../res/train-labels.idx1-ubyte";

	ifstream lab_ifs(labelFileName, ios_base::binary);
	ifstream ifs(fileName, ios_base::binary);

	if( ifs.fail() == true )
		return -1;

	if( lab_ifs.fail() == true )
		return -1;

	//Read train data number and image rows / cols
	char magicNum[4], ccount[4], crows[4], ccols[4];
	ifs.read(magicNum, sizeof(magicNum));
	ifs.read(ccount, sizeof(ccount));
	ifs.read(crows, sizeof(crows));
	ifs.read(ccols, sizeof(ccols));

	int count, rows, cols;
	swapBuffer(ccount);
	swapBuffer(crows);
	swapBuffer(ccols);

	memcpy(&count, ccount, sizeof(count));
	memcpy(&rows, crows, sizeof(rows));
	memcpy(&cols, ccols, sizeof(cols));

	//Just skip label header
	lab_ifs.read(magicNum, sizeof(magicNum));
	lab_ifs.read(ccount, sizeof(ccount));

	//Create source and show image matrix
	Mat src = Mat::zeros(rows, cols, CV_8UC1);
	Mat temp = Mat::zeros(8, 8, CV_8UC1);
	Mat img, dst;

	char label = 0;
	Scalar templateColor(255, 0, 255 );

	NumTrainData rtd;

	//int loop = 1000;
	int total = 0;

	while(!ifs.eof())
	{
		if(total >= count)
			break;

		total++;
		cout << total << endl;

		//Read label
		lab_ifs.read(&label, 1);
		label = label + '0';

		//Read source data
		ifs.read((char*)src.data, rows * cols);
		GetROI(src, dst);

#if(SHOW_PROCESS)
		//Too small to watch
		img = Mat::zeros(dst.rows*10, dst.cols*10, CV_8UC1);
		resize(dst, img, img.size());

		stringstream ss;
		ss << "Number " << label;
		string text = ss.str();
		putText(img, text, Point(10, 50), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);

		//imshow("img", img);
#endif

		rtd.result = label;
		resize(dst, temp, temp.size());
		//threshold(temp, temp, 10, 1, CV_THRESH_BINARY);

		for(int i = 0; i<8; i++)
		{
			for(int j = 0; j<8; j++)
			{
					rtd.data[ i*8 + j] = temp.at<uchar>(i, j);
			}
		}

		buffer.push_back(rtd);

		//if(waitKey(0)==27) //ESC to quit
		//	break;

		maxCount--;

		if(maxCount == 0)
			break;
	}

	ifs.close();
	lab_ifs.close();

	return 0;
}

void newRtStudy(vector<NumTrainData>& trainData)
{
	int testCount = trainData.size();

	Mat data = Mat::zeros(testCount, featureLen, CV_32FC1);
	Mat res = Mat::zeros(testCount, 1, CV_32SC1);

	for (int i= 0; i< testCount; i++)
	{

		NumTrainData td = trainData.at(i);
		memcpy(data.data + i*featureLen*sizeof(float), td.data, featureLen*sizeof(float));

		res.at<unsigned int>(i, 0) = td.result;
	}

	/////////////START RT TRAINNING//////////////////
	RTrees* forest = RTrees::create();
	Ptr<ANN_MLP>model = ANN_MLP::create();

	CvMat* var_importance = 0;

	forest->setMaxDepth(10);
	forest->setMinSampleCount(10);
	forest->setRegressionAccuracy(0);

	forest->setUseSurrogates(false);
	forest->setMaxCategories(15);
	forest->setPriors(Mat());
	forest->setCalculateVarImportance(true);
	forest->setActiveVarCount(4);
	forest->setTermCriteria(TC(100, 0.01f));

	//TODO:://forest->train( &data);
	forest->save( "new_rtrees.xml" );
}


int newRtPredict()
{
	RTrees* forest = RTrees::create();
	forest->load( "new_rtrees.xml" );

	const char fileName[] = "../res/t10k-images.idx3-ubyte";
	const char labelFileName[] = "../res/t10k-labels.idx1-ubyte";

	ifstream lab_ifs(labelFileName, ios_base::binary);
	ifstream ifs(fileName, ios_base::binary);

	if( ifs.fail() == true )
		return -1;

	if( lab_ifs.fail() == true )
		return -1;

	char magicNum[4], ccount[4], crows[4], ccols[4];
	ifs.read(magicNum, sizeof(magicNum));
	ifs.read(ccount, sizeof(ccount));
	ifs.read(crows, sizeof(crows));
	ifs.read(ccols, sizeof(ccols));

	int count, rows, cols;
	swapBuffer(ccount);
	swapBuffer(crows);
	swapBuffer(ccols);

	memcpy(&count, ccount, sizeof(count));
	memcpy(&rows, crows, sizeof(rows));
	memcpy(&cols, ccols, sizeof(cols));

	Mat src = Mat::zeros(rows, cols, CV_8UC1);
	Mat temp = Mat::zeros(8, 8, CV_8UC1);
	Mat m = Mat::zeros(1, featureLen, CV_32FC1);
	Mat img, dst;

	//Just skip label header
	lab_ifs.read(magicNum, sizeof(magicNum));
	lab_ifs.read(ccount, sizeof(ccount));

	char label = 0;
	Scalar templateColor(255, 0, 0);

	NumTrainData rtd;

	int right = 0, error = 0, total = 0;
	int right_1 = 0, error_1 = 0, right_2 = 0, error_2 = 0;
	while(ifs.good())
	{
		//Read label
		lab_ifs.read(&label, 1);
		label = label + '0';

		//Read data
		ifs.read((char*)src.data, rows * cols);
		GetROI(src, dst);

		//Too small to watch
		img = Mat::zeros(dst.rows*30, dst.cols*30, CV_8UC3);
		resize(dst, img, img.size());

		rtd.result = label;
		resize(dst, temp, temp.size());
		//threshold(temp, temp, 10, 1, CV_THRESH_BINARY);
		for(int i = 0; i<8; i++)
		{
			for(int j = 0; j<8; j++)
			{
					m.at<float>(0,j + i*8) = temp.at<uchar>(i, j);
			}
		}

		if(total >= count)
			break;

		char ret = (char)forest->predict(m);

		if(ret == label)
		{
			right++;
			if(total <= 5000)
				right_1++;
			else
				right_2++;
		}
		else
		{
			error++;
			if(total <= 5000)
				error_1++;
			else
				error_2++;
		}

		total++;

#if(SHOW_PROCESS)
		stringstream ss;
		ss << "Number " << label << ", predict " << ret;
		string text = ss.str();
		putText(img, text, Point(10, 50), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);

		imshow("img", img);
		if(waitKey(0)==27) //ESC to quit
			break;
#endif

	}

	ifs.close();
	lab_ifs.close();

	stringstream ss;
	ss << "Total " << total << ", right " << right <<", error " << error;
	string text = ss.str();
	putText(img, text, Point(50, 50), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
	imshow("img", img);
	waitKey(0);

	return 0;
}

void newSvmStudy(vector<NumTrainData>& trainData)
{
	int testCount = trainData.size();

	Mat m = Mat::zeros(1, featureLen, CV_32FC1);
	Mat data = Mat::zeros(testCount, featureLen, CV_32FC1);
	Mat res = Mat::zeros(testCount, 1, CV_32SC1);

	for (int i= 0; i< testCount; i++)
	{

		NumTrainData td = trainData.at(i);
		memcpy(m.data, td.data, featureLen*sizeof(float));
		normalize(m, m);
		memcpy(data.data + i*featureLen*sizeof(float), m.data, featureLen*sizeof(float));

		res.at<unsigned int>(i, 0) = td.result;
	}

	/////////////START SVM TRAINNING//////////////////
	SVM *svm = SVM::create();

	TermCriteria criteria;

	criteria= TermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	//param= SVMParams(SVM::C_SVC, SVM::RBF, 10.0, 8.0, 1.0, 10.0, 0.5, 0.1, NULL, criteria);


	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setC(1);
	//TODO:://svm->train(&data);

	svm->save( "SVM_DATA.xml" );
}


int newSvmPredict()
{
	SVM* svm = SVM::create();
	svm->load( "SVM_DATA.xml" );

	const char fileName[] = "../res/t10k-images.idx3-ubyte";
	const char labelFileName[] = "../res/t10k-labels.idx1-ubyte";

	ifstream lab_ifs(labelFileName, ios_base::binary);
	ifstream ifs(fileName, ios_base::binary);

	if( ifs.fail() == true )
		return -1;

	if( lab_ifs.fail() == true )
		return -1;

	char magicNum[4], ccount[4], crows[4], ccols[4];
	ifs.read(magicNum, sizeof(magicNum));
	ifs.read(ccount, sizeof(ccount));
	ifs.read(crows, sizeof(crows));
	ifs.read(ccols, sizeof(ccols));

	int count, rows, cols;
	swapBuffer(ccount);
	swapBuffer(crows);
	swapBuffer(ccols);

	memcpy(&count, ccount, sizeof(count));
	memcpy(&rows, crows, sizeof(rows));
	memcpy(&cols, ccols, sizeof(cols));

	Mat src = Mat::zeros(rows, cols, CV_8UC1);
	Mat temp = Mat::zeros(8, 8, CV_8UC1);
	Mat m = Mat::zeros(1, featureLen, CV_32FC1);
	Mat img, dst;

	//Just skip label header
	lab_ifs.read(magicNum, sizeof(magicNum));
	lab_ifs.read(ccount, sizeof(ccount));

	char label = 0;
	Scalar templateColor(255, 0, 0);

	NumTrainData rtd;

	int right = 0, error = 0, total = 0;
	int right_1 = 0, error_1 = 0, right_2 = 0, error_2 = 0;
	while(ifs.good())
	{
		//Read label
		lab_ifs.read(&label, 1);
		label = label + '0';

		//Read data
		ifs.read((char*)src.data, rows * cols);
		GetROI(src, dst);

		//Too small to watch
		img = Mat::zeros(dst.rows*30, dst.cols*30, CV_8UC3);
		resize(dst, img, img.size());

		rtd.result = label;
		resize(dst, temp, temp.size());
		//threshold(temp, temp, 10, 1, CV_THRESH_BINARY);
		for(int i = 0; i<8; i++)
		{
			for(int j = 0; j<8; j++)
			{
					m.at<float>(0,j + i*8) = temp.at<uchar>(i, j);
			}
		}

		if(total >= count)
			break;

		normalize(m, m);
		char ret = (char)svm->predict(m);

		if(ret == label)
		{
			right++;
			if(total <= 5000)
				right_1++;
			else
				right_2++;
		}
		else
		{
			error++;
			if(total <= 5000)
				error_1++;
			else
				error_2++;
		}

		total++;

#if(SHOW_PROCESS)
		stringstream ss;
		ss << "Number " << label << ", predict " << ret;
		string text = ss.str();
		putText(img, text, Point(10, 50), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);

		imshow("img", img);
		if(waitKey(0)==27) //ESC to quit
			break;
#endif

	}

	ifs.close();
	lab_ifs.close();

	stringstream ss;
	ss << "Total " << total << ", right " << right <<", error " << error;
	string text = ss.str();
	putText(img, text, Point(50, 50), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
	imshow("img", img);
	waitKey(0);

	return 0;
}

int main1( int argc, char *argv[] )
{
#if(ON_STUDY)
	int maxCount = 60000;
	ReadTrainData(maxCount);

	//newRtStudy(buffer);
	newSvmStudy(buffer);
#else
	//newRtPredict();
	newSvmPredict();
#endif
	return 0;
}


//---------------------------------------

#include <iostream>
#include <cstdio>
using namespace std;
using namespace cv;
using namespace cv::ml;
