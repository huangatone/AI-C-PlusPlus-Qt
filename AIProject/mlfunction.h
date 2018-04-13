#ifndef MLFUNCTION_H
#define MLFUNCTION_H


#include <opencv/cv.h>
#include <opencv/cvwimage.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
//#include <opencv2/tracking.hpp>

#include <QImage>
using namespace std;
using namespace cv;
using namespace ml;

//using Mat to QImage
#include "cvfunction.h"

class MLFunction
{
public:
	MLFunction();

	//using picture to train a model and save it to file
	int train();
	//using traided model( Function train()) to find result
	int doWork(string file);
	//using a picture to make a model and find result; No ML
	int NumberFunction();
	int findNumber();


	// 读取文件数据
	bool read_num_class_data(const string& filename, int var_count,Mat* _data, Mat* _responses);
	//准备训练数据
	Ptr<TrainData> prepare_train_data(const Mat& data, const Mat& responses, int ntrain_samples);

	//分类预测
	void test_and_save_classifier(const Ptr<StatModel>& model,    const Mat& data, const Mat& responses,
		int ntrain_samples, int rdelta);

	//随机树分类
	bool build_rtrees_classifier(const string& data_filename);

	//adaboost分类
	bool build_boost_classifier(const string& data_filename);

	//多层感知机分类（ANN）
	bool build_mlp_classifier(const string& data_filename);
	//K最近邻分类
	bool build_knearest_classifier(const string& data_filename, int K);
	//贝叶斯分类
	bool build_nbayes_classifier(const string& data_filename);
	//svm分类
	bool build_svm_classifier(const string& data_filename);

	int buildAll();

private:
	CVFunction _CVFun;
};

#endif // MLFUNCTION_H
