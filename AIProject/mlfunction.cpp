#include "mlfunction.h"


#include <opencv2/tracking.hpp>
#include <QDebug>
#include <QMessageBox>

#include <map>
#include <vector>
#include <QFile>
#include <QDir>
#include "opencv2/text.hpp"

using namespace cv::text;
using namespace std;

MLFunction::MLFunction()
{

}


struct contours_rect{
	double x,y;                    //轮廓位置
	int order;                      //轮廓向量contours中的第几个
};


bool compare_rect (contours_rect& m1 , contours_rect &m2){
	if(m1.y > m2.y) return false;
	else  if( m1.y == m2.y){
		if(m1.x < m2.x) return true;
		else return false;
	}
	else return true;
}

struct contours_result{
	double bi;
	int num;


};
contours_result result[15];

bool compare_result(contours_result &m1, contours_result &m2)
{
	if(m1.bi < m2.bi)return true;
		else return false;
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

void Threshold(Mat &src,Mat &sample ,int m)
{
	cvtColor(sample, sample, COLOR_BGR2GRAY);
	threshold(sample, sample, 48, 255, CV_THRESH_BINARY_INV);

	//TODO: add more
	result[m].bi = compare(src,sample);
	result[m].num = m;
}

void deal(Mat &src,int order)
{
	Mat sample;
	sample = imread("/rong/outpng/0.png");
	Threshold(src,sample,0);

	sample = imread("/rong/outpng/1.png");
	Threshold(src,sample,1);

	sample = imread("/rong/outpng/2.png");
	Threshold(src,sample,2);

	sample = imread("/rong/outpng/3.png");
	Threshold(src,sample,3);

	sample = imread("/rong/outpng/4.png");
	Threshold(src,sample,4);

	sample = imread("/rong/outpng/5.png");
	Threshold(src,sample,5);

	sample = imread("/rong/outpng/6.png");
	Threshold(src,sample,6);

	sample = imread("/rong/outpng/7.png");
	Threshold(src,sample,7);

	sample = imread("/rong/outpng/8.png");
	Threshold(src,sample,8);

	sample = imread("/rong/outpng/9.png");
	Threshold(src,sample,9);

	sort(result,result+10,compare_result);

	if(result[9].bi > 0.6)
	{
		cout << "第" << order << "个数字为 "<< result[9]. num<<endl;
		cout << "识别精度为 " << result[9].bi <<endl;

		//Mat2QImage1(src).save( QString("/rong/outpng/out_result_%1%2.png").arg( result[9].num ).arg(QTime::currentTime().toString("hhmmss.zzz") ));

	}
	else cout << "第" << order << "个数字无法识别"<<endl;
}

int MLFunction::train()
{
	////==========================读取图片创建训练数据==============================////
	//将所有图片大小统一转化为8*16
	const int imageRows = 8;
	const int imageCols = 16;
	//图片共有10类
	const int classSum = 10;
	//每类共50张图片
	const int imagesSum = 50;
	//每一行一个训练图片
	float trainingData[classSum*imagesSum][imageRows*imageCols] = {{0}};
	//训练样本标签
	float labels[classSum*imagesSum][classSum]={{0}};
	Mat src, resizeImg, trainImg;
	for (int i = 0; i < classSum; i++)
	{
		//目标文件夹路径
		std::string inPath = "/rong/traindata/charSamples/";
		char temp[256];
		int k = 0;
		sprintf(temp, "%d", i);
		inPath = inPath + temp ;
		//check file existed
		QDir dir(QString::fromStdString(inPath));
		//QFile fi( QString::fromStdString(inPath));
		if( dir.exists() == false)
			return -1;
        QFileInfoList file_infos = dir.entryInfoList();

        foreach (QFileInfo info, file_infos) {

			//找到的文件的文件名
			if(info.isFile() == false)
				continue;
			qDebug() << info.filePath();

			src = imread(info.filePath().toStdString(), 0);
			if (src.empty())
			{
				std::cout<<"can not load image \n"<<std::endl;
				return -1;
			}
			//将所有图片大小统一转化为8*16
			resize(src, resizeImg, Size(imageRows,imageCols), (0,0), (0,0), INTER_AREA);
			threshold(resizeImg, trainImg,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
			for(int j = 0; j<imageRows*imageCols; j++)
			{
				trainingData[i*imagesSum + k][j] = (float)resizeImg.data[j];
			}
			// 设置标签数据
			for(int j = 0;j < classSum; j++)
			{
				if(j == i)
					labels[i*imagesSum + k][j] = 1;
				else
					labels[i*imagesSum + k][j] = 0;
			}
			k++;

		}

		Mat labelsMat(classSum*imagesSum, classSum, CV_32FC1,labels);


	}
	//训练样本数据及对应标签
	Mat trainingDataMat(classSum*imagesSum, imageRows*imageCols, CV_32FC1, trainingData);
	Mat labelsMat(classSum*imagesSum, classSum, CV_32FC1, labels);
	//std::cout<<"trainingDataMat: \n"<<trainingDataMat<<"\n"<<std::endl;
	//std::cout<<"labelsMat: \n"<<labelsMat<<"\n"<<std::endl;
	////==========================训练部分==============================////

	Ptr<ANN_MLP>model = ANN_MLP::create();
	Mat layerSizes = (Mat_<int>(1,5)<<imageRows*imageCols,128,128,128,classSum);
	model->setLayerSizes(layerSizes);
	model->setTrainMethod(ANN_MLP::BACKPROP, 0.001, 0.1);
	model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1.0, 1.0);
	model->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000,0.0001));

	Ptr<TrainData> trainData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	model->train(trainData);
	//保存训练结果
	model->save("/rong/traindata/MLPModel.xml");

	////==========================预测部分==============================////
	//读取测试图像
	Mat test, dst;
	test = imread("/rong/traindata/charSamples/5/9_0.975088_gray_23754_9790_step5_recog_6_5_0.996733_0.971903.png", 0);;
	if (test.empty())
	{
		std::cout<<"can not load image \n"<<std::endl;
		return -1;
	}
	//将测试图像转化为1*128的向量
	resize(test, test, Size(imageRows,imageCols), (0,0), (0,0), INTER_AREA);
	threshold(test, test, 0, 255, CV_THRESH_BINARY|CV_THRESH_OTSU);
	Mat_<float> testMat(1, imageRows*imageCols);
	for (int i = 0; i < imageRows*imageCols; i++)
	{
		testMat.at<float>(0,i) = (float)test.at<uchar>(i/8, i%8);
	}
	//使用训练好的MLP model预测测试图像
	model->predict(testMat, dst);
	std::cout<<"testMat: \n"<<testMat<<"\n"<<std::endl;
	std::cout<<"dst: \n"<<dst<<"\n"<<std::endl;
	double maxVal = 0;
	Point maxLoc;
	minMaxLoc(dst, NULL, &maxVal, NULL, &maxLoc);
	std::cout<<"测试结果："<<maxLoc.x<<"置信度:"<<maxVal*100<<"%"<<std::endl;
	imshow("test",test);
	waitKey(0);
	return 0;
}


//利用训练完成的神经网络模型进行识别
int MLFunction::doWork(string file)
{
	//将所有图片大小统一转化为8*16
	const int imageRows = 8;
	const int imageCols = 16;
	//读取训练结果
	Ptr<ANN_MLP> model = StatModel::load<ANN_MLP>("/rong/traindata/MLPModel.xml");// old 1
	//Ptr<ANN_MLP> model = StatModel::load<ANN_MLP>(file);
	////==========================预测部分==============================////
	//读取测试图像
	Mat test, dst;
	//test = imread("E:/image/image/test.png", 0); old 1
	test = imread(file, 0);
	if (test.empty())
	{
		std::cout<<"do work can not load image \n"<<std::endl;
		return -1;
	}
	//将测试图像转化为1*128的向量
	resize(test, test, Size(imageRows,imageCols), (0,0), (0,0), INTER_AREA);
	threshold(test, test, 0, 255, CV_THRESH_BINARY|CV_THRESH_OTSU);
	Mat_<float> testMat(1, imageRows*imageCols);
	for (int i = 0; i < imageRows*imageCols; i++)
	{
		testMat.at<float>(0,i) = (float)test.at<uchar>(i/8, i%8);
	}
	//使用训练好的MLP model预测测试图像
	model->predict(testMat, dst);
	std::cout<<"testMat: \n"<<testMat<<"\n"<<std::endl;
	std::cout<<"dst: \n"<<dst<<"\n"<<std::endl;
	double maxVal = 0;
	Point maxLoc;
	minMaxLoc(dst, NULL, &maxVal, NULL, &maxLoc);
	std::cout<<"测试结果："<<maxLoc.x<<"置信度:"<<maxVal*100<<"%"<<std::endl;
	//imshow("test",test);
	//waitKey(0);
	return 0;
}



int MLFunction::NumberFunction()
{
	Mat src = imread("/rong/outpng/ss.png");

	Mat srcGray;
	cv::cvtColor(src, srcGray,COLOR_RGB2GRAY);
	_CVFun.Mat2QImage1(srcGray).save("/rong/outpng/gray.png");
	Mat bl;
	//blur( srcGray, bl, Size(5,5) );
	cv::GaussianBlur(srcGray,bl,Size(5,5),0);
	_CVFun.Mat2QImage1(bl).save("/rong/outpng/blue.png");
	Mat threMat;
	//阈值化操作
	threshold(srcGray, threMat, 100, 255, THRESH_BINARY);

	std::vector<std::vector<cv::Point> > contours;

	vector<Vec4i> hierarchy;

	findContours(threMat, contours,hierarchy, CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);

	//绘出轮廓
	RNG rng(12345);

	QList<Rect> ltRect;
	QList<contours_rect> con;
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	int i = 0;
	for (i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255),rng.uniform(0,255),rng.uniform(0,255));
		Point2f vertex[4];
		//------rotate rect
		/*
		RotatedRect rt = minAreaRect(contours[i]);

		rt.points(vertex);

		//if( rt.area() > 58  )
		//	rectangle(src, rt, Scalar(0,255,255), 1);
		 for( int j = 0; j < 4; j++)
				line(src,vertex[j], vertex[ (j+1)%4 ],Scalar(0,0,255),1);

		*/

		//  no rotate rect.
		//画出可包围数字的最小矩形

		Rect rt = boundingRect(contours[i]);
		if( rt.area() < 500  )
			continue;

		ltRect << rt;
		vertex[0] = rt.tl();                                                              //矩阵左上角的点
		vertex[1].x = (float)rt.tl().x, vertex[1].y = (float)rt.br().y;                 //矩阵左下方的点
		vertex[2] = rt.br();                                                              //矩阵右下角的点
		vertex[3].x = (float)rt.br().x, vertex[3].y = (float)rt.tl().y;                 //矩阵右上方的点
		for( int j = 0; j < 4; j++)
			   line(drawing,vertex[j], vertex[ (j+1)%4 ],Scalar(0,0,255),1);
		//矩阵右上方的点
		//随机颜色绘制轮廓
		//drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());

		contours_rect cc;

		cc.x = (vertex[0].x+vertex[1].x+vertex[2].x+vertex[3].x) / 4.0;                 //根据中心点判断图像的位置
		cc.y = (vertex[0].y+vertex[1].y+vertex[2].y+vertex[3].y) / 4.0;

		//cout << i <<":"<< endl;
		//cout << vertex[3].x<<"  "<< vertex[3].y<<endl;
		cc.order = i;
		con << cc;


	}

	_CVFun.Mat2QImage(drawing).save("/rong/outpng/result.png");


	//3.数字顺序整理：
	//sort(con.begin(),con.end(), compare_rect);

	//4.切割各个数字：
	//method 1

	QList<Mat> num;

	qDebug() << "countour" << i;

	for(int j = 0; j < i , j < con.size(); j++){

		qDebug() <<"area == " <<  ltRect[j].area() ;
		if( ltRect[j].area() < 500  )
			continue;

		   Mat t;
		   src(ltRect[j]).copyTo(t);
		   _CVFun.Mat2QImage1(t).save( QString("/rong/outpng/result_%1.png").arg(j));
		   cvtColor(t, t, COLOR_BGR2GRAY);
		   threshold(t, t, 48, 255, CV_THRESH_BINARY_INV);
		   deal(t,j+1);
		   num << t;

	   }

	//method 2
	/*Mat num[10];
		for(int j = 0; j < i; j++){
			cout << "s "<<j<<endl;
			int k = con[j].order;
			cout << "k "<<k<<endl;
			srcImage(rect[k]).copyTo(num[j]);
		}
*/

	return 0;
}



// 读取文件数据
bool MLFunction::read_num_class_data(const string& filename, int var_count,Mat* _data, Mat* _responses)
{
	const int M = 1024;
	char buf[M + 2];

	Mat el_ptr(1, var_count, CV_32F);
	int i;
	vector<int> responses;

	_data->release();
	_responses->release();
	FILE *f =	fopen( filename.c_str(), "rt");
	if (!f)
	{
		cout << "Could not read the database " << filename << endl;
		return false;
	}

	for (;;)
	{
		char* ptr;
		if (!fgets(buf, M, f) || !strchr(buf, ','))
			break;
		responses.push_back((int)buf[0]);
		ptr = buf + 2;
		for (i = 0; i < var_count; i++)
		{
			int n = 0;
			sscanf(ptr, "%f%n", &el_ptr.at<float>(i), &n);
			ptr += n + 1;
		}
		if (i < var_count)
			break;
		_data->push_back(el_ptr);
	}
	fclose(f);
	Mat(responses).copyTo(*_responses);
	return true;
}


//准备训练数据
Ptr<TrainData> MLFunction::MLFunction::prepare_train_data(const Mat& data, const Mat& responses, int ntrain_samples)
{
	Mat sample_idx = Mat::zeros(1, data.rows, CV_8U);
	Mat train_samples = sample_idx.colRange(0, ntrain_samples);
	train_samples.setTo(Scalar::all(1));

	int nvars = data.cols;
	Mat var_type(nvars + 1, 1, CV_8U);
	var_type.setTo(Scalar::all(VAR_ORDERED));
	var_type.at<uchar>(nvars) = VAR_CATEGORICAL;

	return TrainData::create(data, ROW_SAMPLE, responses,
		noArray(), sample_idx, noArray(), var_type);
}



//分类预测
void MLFunction::test_and_save_classifier(const Ptr<StatModel>& model,    const Mat& data, const Mat& responses,
	int ntrain_samples, int rdelta)
{
	int i, nsamples_all = data.rows;
	double train_hr = 0, test_hr = 0;

	// compute prediction error on train and test data
	for (i = 0; i < nsamples_all; i++)
	{
		Mat sample = data.row(i);

		float r = model->predict(sample);
		r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;

		if (i < ntrain_samples)
			train_hr += r;
		else
			test_hr += r;
	}

	test_hr /= nsamples_all - ntrain_samples;
	train_hr = ntrain_samples > 0 ? train_hr / ntrain_samples : 1.;

	printf("Recognition rate: train = %.1f%%, test = %.1f%%\n",
		train_hr*100., test_hr*100.);

}

//随机树分类
bool MLFunction::build_rtrees_classifier(const string& data_filename)
{
	Mat data;
	Mat responses;
	read_num_class_data(data_filename, 16, &data, &responses);

	int nsamples_all = data.rows;
	int ntrain_samples = (int)(nsamples_all*0.8);

	Ptr<RTrees> model;
	Ptr<TrainData> tdata = prepare_train_data(data, responses, ntrain_samples);
	model = RTrees::create();
	model->setMaxDepth(10);
	model->setMinSampleCount(10);
	model->setRegressionAccuracy(0);
	model->setUseSurrogates(false);
	model->setMaxCategories(15);
	model->setPriors(Mat());
	model->setCalculateVarImportance(true);
	model->setActiveVarCount(4);
	model->setTermCriteria(TC(100, 0.01f));
	model->train(tdata);
	test_and_save_classifier(model, data, responses, ntrain_samples, 0);
	cout << "Number of trees: " << model->getRoots().size() << endl;

	// Print variable importance
	Mat var_importance = model->getVarImportance();
	if (!var_importance.empty())
	{
		double rt_imp_sum = sum(var_importance)[0];
		printf("var#\timportance (in %%):\n");
		int i, n = (int)var_importance.total();
		for (i = 0; i < n; i++)
			printf("%-2d\t%-4.1f\n", i, 100.f*var_importance.at<float>(i) / rt_imp_sum);
	}

	return true;
}

//adaboost分类
bool MLFunction::build_boost_classifier(const string& data_filename)
{
	const int class_count = 26;
	Mat data;
	Mat responses;
	Mat weak_responses;

	read_num_class_data(data_filename, 16, &data, &responses);
	int i, j, k;
	Ptr<Boost> model;

	int nsamples_all = data.rows;
	int ntrain_samples = (int)(nsamples_all*0.5);
	int var_count = data.cols;

	Mat new_data(ntrain_samples*class_count, var_count + 1, CV_32F);
	Mat new_responses(ntrain_samples*class_count, 1, CV_32S);

	for (i = 0; i < ntrain_samples; i++)
	{
		const float* data_row = data.ptr<float>(i);
		for (j = 0; j < class_count; j++)
		{
			float* new_data_row = (float*)new_data.ptr<float>(i*class_count + j);
			memcpy(new_data_row, data_row, var_count*sizeof(data_row[0]));
			new_data_row[var_count] = (float)j;
			new_responses.at<int>(i*class_count + j) = responses.at<int>(i) == j + 'A';
		}
	}

	Mat var_type(1, var_count + 2, CV_8U);
	var_type.setTo(Scalar::all(VAR_ORDERED));
	var_type.at<uchar>(var_count) = var_type.at<uchar>(var_count + 1) = VAR_CATEGORICAL;

	Ptr<TrainData> tdata = TrainData::create(new_data, ROW_SAMPLE, new_responses,
		noArray(), noArray(), noArray(), var_type);
	vector<double> priors(2);
	priors[0] = 1;
	priors[1] = 26;

	model = Boost::create();
	model->setBoostType(Boost::GENTLE);
	model->setWeakCount(100);
	model->setWeightTrimRate(0.95);
	model->setMaxDepth(5);
	model->setUseSurrogates(false);
	model->setPriors(Mat(priors));
	model->train(tdata);
	Mat temp_sample(1, var_count + 1, CV_32F);
	float* tptr = temp_sample.ptr<float>();

	// compute prediction error on train and test data
	double train_hr = 0, test_hr = 0;
	for (i = 0; i < nsamples_all; i++)
	{
		int best_class = 0;
		double max_sum = -DBL_MAX;
		const float* ptr = data.ptr<float>(i);
		for (k = 0; k < var_count; k++)
			tptr[k] = ptr[k];

		for (j = 0; j < class_count; j++)
		{
			tptr[var_count] = (float)j;
			float s = model->predict(temp_sample, noArray(), StatModel::RAW_OUTPUT);
			if (max_sum < s)
			{
				max_sum = s;
				best_class = j + 'A';
			}
		}

		double r = std::abs(best_class - responses.at<int>(i)) < FLT_EPSILON ? 1 : 0;
		if (i < ntrain_samples)
			train_hr += r;
		else
			test_hr += r;
	}

	test_hr /= nsamples_all - ntrain_samples;
	train_hr = ntrain_samples > 0 ? train_hr / ntrain_samples : 1.;
	printf("Recognition rate: train = %.1f%%, test = %.1f%%\n",
		train_hr*100., test_hr*100.);

	cout << "Number of trees: " << model->getRoots().size() << endl;
	return true;
}

//多层感知机分类（ANN）
bool MLFunction::build_mlp_classifier(const string& data_filename)
{
	const int class_count = 26;
	Mat data;
	Mat responses;

	read_num_class_data(data_filename, 16, &data, &responses);
	Ptr<ANN_MLP> model;

	int nsamples_all = data.rows;
	int ntrain_samples = (int)(nsamples_all*0.8);
	Mat train_data = data.rowRange(0, ntrain_samples);
	Mat train_responses = Mat::zeros(ntrain_samples, class_count, CV_32F);

	// 1. unroll the responses
	cout << "Unrolling the responses...\n";
	for (int i = 0; i < ntrain_samples; i++)
	{
		int cls_label = responses.at<int>(i) -'A';
		train_responses.at<float>(i, cls_label) = 1.f;
	}

	// 2. train classifier
	int layer_sz[] = { data.cols, 100, 100, class_count };
	int nlayers = (int)(sizeof(layer_sz) / sizeof(layer_sz[0]));
	Mat layer_sizes(1, nlayers, CV_32S, layer_sz);

#if 1
	int method = ANN_MLP::BACKPROP;
	double method_param = 0.001;
	int max_iter = 300;
#else
	int method = ANN_MLP::RPROP;
	double method_param = 0.1;
	int max_iter = 1000;
#endif

	Ptr<TrainData> tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);
	model = ANN_MLP::create();
	model->setLayerSizes(layer_sizes);
	model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 0, 0);
	model->setTermCriteria(TC(max_iter, 0));
	model->setTrainMethod(method, method_param);
	model->train(tdata);
	return true;
}

//K最近邻分类
bool MLFunction::build_knearest_classifier(const string& data_filename, int K)
{
	Mat data;
	Mat responses;
	read_num_class_data(data_filename, 16, &data, &responses);
	int nsamples_all = data.rows;
	int ntrain_samples = (int)(nsamples_all*0.8);

	Ptr<TrainData> tdata = prepare_train_data(data, responses, ntrain_samples);
	Ptr<KNearest> model = KNearest::create();
	model->setDefaultK(K);
	model->setIsClassifier(true);
	model->train(tdata);

	test_and_save_classifier(model, data, responses, ntrain_samples, 0);
	return true;
}

//贝叶斯分类
bool MLFunction::build_nbayes_classifier(const string& data_filename)
{
	Mat data;
	Mat responses;
	read_num_class_data(data_filename, 16, &data, &responses);

	int nsamples_all = data.rows;
	int ntrain_samples = (int)(nsamples_all*0.8);

	Ptr<NormalBayesClassifier> model;
	Ptr<TrainData> tdata = prepare_train_data(data, responses, ntrain_samples);
	model = NormalBayesClassifier::create();
	model->train(tdata);

	test_and_save_classifier(model, data, responses, ntrain_samples, 0);
	return true;
}


//svm分类
bool MLFunction::build_svm_classifier(const string& data_filename)
{
	Mat data;
	Mat responses;
	read_num_class_data(data_filename, 16, &data, &responses);

	int nsamples_all = data.rows;
	int ntrain_samples = (int)(nsamples_all*0.8);

	Ptr<SVM> model;
	Ptr<TrainData> tdata = prepare_train_data(data, responses, ntrain_samples);
	model = SVM::create();
	model->setType(SVM::C_SVC);
	model->setKernel(SVM::LINEAR);
	model->setC(1);
	model->train(tdata);

	test_and_save_classifier(model, data, responses, ntrain_samples, 0);
	return true;
}

int MLFunction::buildAll()
{
	string data_filename = "/rong/traindata/letter-recognition.data";  //字母数据

	cout << "svm分类：" << endl;
	build_svm_classifier(data_filename);

	cout << "贝叶斯分类：" << endl;
	build_nbayes_classifier(data_filename);

	cout << "K最近邻分类：" << endl;
	build_knearest_classifier(data_filename,10);

	cout << "随机树分类：" << endl;
	build_rtrees_classifier(data_filename);

	//cout << "adaboost分类：" << endl;
	//build_boost_classifier(data_filename);

	//cout << "ANN（多层感知机)分类：" << endl;
	//build_mlp_classifier(data_filename);
}








int MLFunction::findNumber()
{
	CVFunction cf;
	Mat src = imread("/rong/outpng/ss.png");

	Mat srcGray;
	cv::cvtColor(src, srcGray,COLOR_RGB2GRAY);
	cf.Mat2QImage1(srcGray).save("/rong/outpng/gray.png");
	Mat bl;
	//blur( srcGray, bl, Size(5,5) );
	cv::GaussianBlur(srcGray,bl,Size(5,5),0);
	cf.Mat2QImage1(bl).save("/rong/outpng/blue.png");
	Mat threMat;
	//阈值化操作
	threshold(srcGray, threMat, 100, 255, THRESH_BINARY);

	std::vector<std::vector<cv::Point> > contours;

	vector<Vec4i> hierarchy;

	findContours(threMat, contours,hierarchy, CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);

	//绘出轮廓
	RNG rng(12345);

	QList<Rect> ltRect;
	QList<contours_rect> con;
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	int i = 0;
	for (i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255),rng.uniform(0,255),rng.uniform(0,255));
		Point2f vertex[4];


		Rect rt = boundingRect(contours[i]);
		if( rt.area() < 500  )
			continue;

		ltRect << rt;
		vertex[0] = rt.tl();                                                              //矩阵左上角的点
		vertex[1].x = (float)rt.tl().x, vertex[1].y = (float)rt.br().y;                 //矩阵左下方的点
		vertex[2] = rt.br();                                                              //矩阵右下角的点
		vertex[3].x = (float)rt.br().x, vertex[3].y = (float)rt.tl().y;                 //矩阵右上方的点

		contours_rect cc;

		cc.x = (vertex[0].x+vertex[1].x+vertex[2].x+vertex[3].x) / 4.0;                 //根据中心点判断图像的位置
		cc.y = (vertex[0].y+vertex[1].y+vertex[2].y+vertex[3].y) / 4.0;


		cc.order = i;
		con << cc;


	}

	cf.Mat2QImage(drawing).save("/rong/outpng/result.png");


	//3.数字顺序整理：
	//sort(con.begin(),con.end(), compare_rect);

	//4.切割各个数字：
	//method 1

	QList<Mat> num;

	qDebug() << "countour" << i;

	for(int j = 0; j < i , j < con.size(); j++){

		qDebug() <<"area == " <<  ltRect[j].area() ;
		if( ltRect[j].area() < 500  )
			continue;

		   Mat t;
		   src(ltRect[j]).copyTo(t);
		   cf.Mat2QImage1(t).save( QString("/rong/outpng/result_%1.png").arg(j));
		   cvtColor(t, t, COLOR_BGR2GRAY);
		   threshold(t, t, 48, 255, CV_THRESH_BINARY_INV);
		   deal(t,j+1);
		   num << t;

	   }



	return 0;
}

