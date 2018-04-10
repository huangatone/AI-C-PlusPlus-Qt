#include "newfun.h"

NewFun::NewFun()
{

}


#include <opencv2/ml.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>

#include <time.h>

using namespace cv;
using namespace std;
using namespace ml;


#define HORIZONTAL    1
#define VERTICAL    0


#include "opencv2/opencv.hpp"
#include "opencv2/ml.hpp"
using namespace cv;
using namespace ml;



int Test1()
{
	//训练数据及对应标签
	float trainingData[8][2] = {{480,500},{50,130},{110,32},{490,60},{60,190},{200,189},{78,256},{45,315}};
	float labels[8] = {0,1,0,0,1,0,1,1};
	Mat trainingDataMat(8, 2, CV_32FC1, trainingData);
	Mat labelsMat(8, 1, CV_32FC1, labels);
	//建立模型
	Ptr<ANN_MLP> model = ANN_MLP::create();
	//共5层：输入层+3个隐藏层+1个输出层，输入层、隐藏层每层均为2个perceptron
	Mat layerSizes = (Mat_<int>(1, 5) << 2, 2, 2, 2, 1);
	//设置各层的神经元个数
	model->setLayerSizes(layerSizes);
	//激活函数
	model->setActivationFunction(ANN_MLP::SIGMOID_SYM);
	//MLP的训练方法
	model->setTrainMethod(ANN_MLP::BACKPROP,0.1,0.9);
	//训练模型
	Ptr<TrainData> trainData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	model->train(trainData);

	Mat src = Mat::zeros(512, 512, CV_8UC3);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			Mat sampleMat = (Mat_<float>(1, 2) << i, j);
			Mat responseMat;
			model->predict(sampleMat, responseMat);
			float p = responseMat.ptr<float>(0)[0];
			if (p> 0.5)
				src.at<Vec3b>(j, i) = Vec3b(0,125,125);
			else
				src.at<Vec3b>(j, i) = Vec3b(125,125,0);
		}
	}
	//绘制出训练数据在图中的分布
	Mat dst1 = src.clone();
	for (int i = 0; i < sizeof(trainingData[0]); i++)
	{
		float q = labels[i];
		//根据训练数据对应的标签不同绘制不同的颜色：1对应红色，0对应绿色
		if (q == 1)
			circle(dst1, Point(trainingData[i][0],trainingData[i][1]), 5, Scalar(0,0,255), -1, 8);
		else
			circle(dst1, Point(trainingData[i][0],trainingData[i][1]), 5, Scalar(0,255,0), -1, 8);
	}
	//在原图像范围内随机生成20个点，并使用训练好的神经网络对其进行预测，并绘制出预测结果
	Mat dst2 = src.clone();
	for (int i = 0; i < 20; i++)
	{

		RNG rngx(i);
		float x = rngx.uniform(0,512);
		RNG rngy(i*i);
		float y = rngy.uniform(0,512);

		Mat trainingDataMat = (Mat_<float>(1,2)<<x,y);
		Mat response;
		model->predict(trainingDataMat,response);
		float q = response.ptr<float>(0)[0];
		if (q > 0.5)
			circle(dst2, Point(x,y), 5, Scalar(0,0,255), -1, 8);
		else
			circle(dst2, Point(x,y), 5, Scalar(0,255,0), -1, 8);
	}

	imshow("output1", dst1);
	imshow("output2", dst2);
	waitKey(0);

	return 0;
}


//建立模型
Ptr<TrainData> ann;

const char strCharacters[] = { '0','1','2','3','4','5',\
'6','7','8','9'};
const int numCharacter = 10;

const int numNeurons = 40;
const int predictSize = 10;


void generateRandom(int n, int test_num, int min, int max, vector<int>*mark_samples)
{
	int range = max - min;
	int index = rand() % range + min;
	if (mark_samples->at(index) == 0)
	{
		mark_samples->at(index) = 1;
		n++;
	}

	if (n < test_num)
		generateRandom(n, test_num, min, max, mark_samples);

}


vector<string> getFiles(const string &folder,
	const bool all /* = true */) {
	vector<string> files;
	list<string> subfolders;
	subfolders.push_back(folder);
/*
	while (!subfolders.empty()) {
		string current_folder(subfolders.back());

		if (*(current_folder.end() - 1) != '/') {
			current_folder.append("/*");
		}
		else {
			current_folder.append("*");
		}

		subfolders.pop_back();

		struct _finddata_t file_info;
		long file_handler = _findfirst(current_folder.c_str(), &file_info);

		while (file_handler != -1) {
			if (all &&
				(!strcmp(file_info.name, ".") || !strcmp(file_info.name, ".."))) {
				if (_findnext(file_handler, &file_info) != 0) break;
				continue;
			}

			if (file_info.attrib & _A_SUBDIR) {
				// it's a sub folder
				if (all) {
					// will search sub folder
					string folder(current_folder);
					folder.pop_back();
					folder.append(file_info.name);

					subfolders.push_back(folder.c_str());
				}
			}
			else {
				// it's a file
				string file_path;
				// current_folder.pop_back();
				file_path.assign(current_folder.c_str()).pop_back();
				file_path.append(file_info.name);

				files.push_back(file_path);
			}

			if (_findnext(file_handler, &file_info) != 0) break;
		}  // while
		_findclose(file_handler);
	}
*/
	return files;
}

void AppendText(string filename, string text)
{
    /*fstream ftxt;
    ftxt.open(filename, ios_base::out | ios_base::app);
	if (ftxt.fail())
	{
		cout << "创建文件失败!" << endl;
		getchar();
	}
	ftxt << text << endl;
    ftxt.close();*/
}

// ！获取垂直和水平方向直方图
Mat ProjectedHistogram(Mat img, int t)
{
	int sz = (t) ? img.rows : img.cols;
	Mat mhist = Mat::zeros(1, sz, CV_32F);

	for (int j = 0; j<sz; j++) {
		Mat data = (t) ? img.row(j) : img.col(j);
		mhist.at<float>(j) = countNonZero(data);    //统计这一行或一列中，非零元素的个数，并保存到mhist中
	}

	//Normalize histogram
	double min, max;
	minMaxLoc(mhist, &min, &max);

	if (max>0)
		mhist.convertTo(mhist, -1, 1.0f / max, 0);//用mhist直方图中的最大值，归一化直方图

	return mhist;
}

Mat features(Mat in, int sizeData)
{
	//Histogram features

	Mat vhist = ProjectedHistogram(in, VERTICAL);
	Mat hhist = ProjectedHistogram(in, HORIZONTAL);
	//Low data feature
	Mat lowData;
	resize(in, lowData, Size(sizeData, sizeData));


	//Last 10 is the number of moments components
	int numCols = vhist.cols + hhist.cols + lowData.cols*lowData.cols;
	//int numCols = vhist.cols + hhist.cols;
	Mat out = Mat::zeros(1, numCols, CV_32F);
	//Asign values to feature,ANN的样本特征为水平、垂直直方图和低分辨率图像所组成的矢量
	int j = 0;
	for (int i = 0; i<vhist.cols; i++)
	{
		out.at<float>(j) = vhist.at<float>(i);
		j++;
	}
	for (int i = 0; i<hhist.cols; i++)
	{
		out.at<float>(j) = hhist.at<float>(i);
		j++;
	}
	for (int x = 0; x<lowData.cols; x++)
	{
		for (int y = 0; y<lowData.rows; y++) {
			out.at<float>(j) = (float)lowData.at<unsigned char>(x, y);
			j++;
		}
	}
	//if(DEBUG)
	//  cout << out << "\n===========================================\n";
	return out;
}

void annTrain(Mat TrainData, Mat classes, int nNeruns)
{
	Mat layers(1, 3, CV_32SC1);
	layers.at<int>(0) = TrainData.cols;
	layers.at<int>(1) = nNeruns;
	layers.at<int>(2) = numCharacter;
	//ann->create(layers, ANN_MLP::SIGMOID_SYM, 1, 1);
	ann =  TrainData::create(layers, ROW_SAMPLE, layers);

	//Prepare trainClases
	//Create a mat with n trained data by m classes
	Mat trainClasses;
	trainClasses.create(TrainData.rows, numCharacter, CV_32FC1);
	for (int i = 0; i < trainClasses.rows; i++)
	{
		for (int k = 0; k < trainClasses.cols; k++)
		{
			//If class of data i is same than a k class
			if (k == classes.at<int>(i))
				trainClasses.at<float>(i, k) = 1;
			else
				trainClasses.at<float>(i, k) = 0;
		}
	}
	Mat weights(1, TrainData.rows, CV_32FC1, Scalar::all(1));

	//Learn classifier
	// ann.train( TrainData, trainClasses, weights );

	//Setup the BPNetwork

	// Set up BPNetwork's parameters
	//ANN_MLP_TrainParams params;
//params.train_method = ANN_MLP_TrainParams::BACKPROP;
	//params.bp_dw_scale = 0.1;
	//params.bp_moment_scale = 0.1;

	//params.train_method=CvANN_MLP_TrainParams::RPROP;
	// params.rp_dw0 = 0.1;
	// params.rp_dw_plus = 1.2;
	// params.rp_dw_minus = 0.5;
	// params.rp_dw_min = FLT_EPSILON;
	// params.rp_dw_max = 50.;

	//ann->train(TrainData, trainClasses);

}

int recog(Mat features)
{
	int result = -1;
	Mat Predict_result(1, numCharacter, CV_32FC1);

	Point maxLoc;
	double maxVal;

	minMaxLoc(Predict_result, 0, &maxVal, 0, &maxLoc);

	return maxLoc.x;
}

float ANN_test(Mat samples_set, Mat sample_labels)
{
	int correctNum = 0;
	float accurate = 0;
	for (int i = 0; i < samples_set.rows; i++)
	{
		int result = recog(samples_set.row(i));
		if (result == sample_labels.at<int>(i))
			correctNum++;
	}
	accurate = (float)correctNum / samples_set.rows;
	return accurate;
}

int saveTrainData()
{
	cout << "Begin saveTrainData" << endl;
	Mat classes;
	Mat trainingDataf5;
	Mat trainingDataf10;
	Mat trainingDataf15;
	Mat trainingDataf20;

	vector<int> trainingLabels;
	string path = "charSamples";

	for (int i = 0; i < numCharacter; i++)
	{
		cout << "Character: " << strCharacters[i] << "\n";
		stringstream ss(stringstream::in | stringstream::out);
		ss << path << "/" << strCharacters[i];

        vector<string> files = getFiles(ss.str(),1);

		int size = files.size();
		for (int j = 0; j < size; j++)
		{
			cout << files[j].c_str() << endl;
			Mat img = imread(files[j].c_str(), 0);
			Mat f5 = features(img, 5);
			Mat f10 = features(img, 10);
			Mat f15 = features(img, 15);
			Mat f20 = features(img, 20);

			trainingDataf5.push_back(f5);
			trainingDataf10.push_back(f10);
			trainingDataf15.push_back(f15);
			trainingDataf20.push_back(f20);
			trainingLabels.push_back(i);            //每一幅字符图片所对应的字符类别索引下标
		}
	}



	trainingDataf5.convertTo(trainingDataf5, CV_32FC1);
	trainingDataf10.convertTo(trainingDataf10, CV_32FC1);
	trainingDataf15.convertTo(trainingDataf15, CV_32FC1);
	trainingDataf20.convertTo(trainingDataf20, CV_32FC1);
	Mat(trainingLabels).copyTo(classes);

	FileStorage fs("train/features_data.xml", FileStorage::WRITE);
	fs << "TrainingDataF5" << trainingDataf5;
	fs << "TrainingDataF10" << trainingDataf10;
	fs << "TrainingDataF15" << trainingDataf15;
	fs << "TrainingDataF20" << trainingDataf20;
	fs << "classes" << classes;
	fs.release();

	cout << "End saveTrainData" << endl;

	return 0;
}


void ANN_Cross_Train_and_Test(int Imagsize, int Layers )
{

	String training;
	Mat TrainingData;
	Mat Classes;

	FileStorage fs;
	fs.open("train/features_data.xml", FileStorage::READ);


	cout << "Begin to ANN_Cross_Train_and_Test " << endl;

	char *txt = new char[50];
	sprintf(txt, "交叉训练，特征维度%d,网络层数%d", 40 + Imagsize * Imagsize, Layers);
	AppendText("output.txt", txt);
	cout << txt << endl;
	stringstream ss(stringstream::in | stringstream::out);
	ss << "TrainingDataF" << Imagsize;
	training = ss.str();

	fs[training] >> TrainingData;
	fs["classes"] >> Classes;
	fs.release();

	float result = 0.0;

	srand(time(NULL));

	vector<int> markSample(TrainingData.rows, 0);

	generateRandom(0, 100, 0, TrainingData.rows - 1, &markSample);

	Mat train_set, train_labels;
	Mat sample_set, sample_labels;

	for (int i = 0; i < TrainingData.rows; i++)
	{
		if (markSample[i] == 1)
		{
			sample_set.push_back(TrainingData.row(i));
			sample_labels.push_back(Classes.row(i));
		}
		else
		{
			train_set.push_back(TrainingData.row(i));
			train_labels.push_back(Classes.row(i));
		}
	}

	annTrain(train_set, train_labels, Layers);

	result = ANN_test(sample_set, sample_labels);


	sprintf(txt, "正确率%f\n", result);
	cout << txt << endl;
	AppendText("output.txt", txt);

	cout << "End the ANN_Cross_Train_and_Test" << endl;

	cout << endl;


}


void ANN_test_Main()
{
	int DigitSize[4] = { 5, 10, 15, 20};
	int LayerNum[14] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 150, 200, 500 };
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			ANN_Cross_Train_and_Test(DigitSize[i], LayerNum[j]);
		}
	}
}


void ANN_saveModel(int _predictsize, int _neurons)
{
	FileStorage fs;
	fs.open("train/features_data.xml", FileStorage::READ);

	Mat TrainingData;
	Mat Classes;

	string training;
	if (1)
	{
		stringstream ss(stringstream::in | stringstream::out);
		ss << "TrainingDataF" << _predictsize;
		training = ss.str();
	}

	fs[training] >> TrainingData;
	fs["classes"] >> Classes;

	//train the Ann
	cout << "Begin to saveModelChar predictSize:" << _predictsize
		<< " neurons:" << _neurons << endl;


	annTrain(TrainingData, Classes, _neurons);



	cout << "End the saveModelChar" << endl;


	string model_name = "train/ann10_40.xml";
	//if(1)
	//{
	//  stringstream ss(stringstream::in | stringstream::out);
	//  ss << "ann_prd" << _predictsize << "_neu"<< _neurons << ".xml";
	//  model_name = ss.str();
	//}

	FileStorage fsTo(model_name, cv::FileStorage::WRITE);

}

int oo()
{
	cout << "To be begin." << endl;

	saveTrainData();

	//ANN_saveModel(10, 40);

	ANN_test_Main();

	cout << "To be end." << endl;
	int end;
	cin >> end;
	return 0;
}
