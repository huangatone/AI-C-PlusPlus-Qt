#include "mycv.h"
using namespace  cv:: xfeatures2d;
#include <iomanip>  // for controlling float print precision
#include <QDebug>



MyCv::MyCv()
{

}



QPoint MatchingMethod( QString src_file, QString dst_file ,int md)
{
	Mat img_src =  imread(src_file.toStdString(),1);	
	Mat img_dst = imread(dst_file.toStdString(),1);
	if(img_src.data == nullptr || img_dst.data==nullptr )
	{
		qDebug() << "wrong file"  << src_file << dst_file;
		return  QPoint();
	}	
	/// Source image to display
	Mat img_display;
	Mat result;
	img_src.copyTo( img_display );
	int match_method = md;

	/// Create the result matrix
	int result_cols =  img_src.cols - img_dst.cols +1;
	int result_rows = img_src.rows - img_dst.rows+1;

	result.create( result_rows, result_cols, CV_32FC1 );

	/// Do the Matching and Normalize
	matchTemplate( img_src, img_dst, result, md );
	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
	{ matchLoc = minLoc; }
	else
	{ matchLoc = maxLoc; }

	/// Show me what you got
	rectangle( img_display, matchLoc, Point( matchLoc.x + img_dst.cols , matchLoc.y + img_dst.rows ), Scalar::all(0), 2, 8, 0 );
	rectangle( result, matchLoc, Point( matchLoc.x + img_dst.cols , matchLoc.y + img_dst.rows ), Scalar::all(0), 2, 8, 0 );

	imwrite("/rong/project/a1.png",img_display);
	imwrite("/rong/project/a2.png",result);

	return QPoint(matchLoc.x,matchLoc.y);
}

int CompareReslut( QPoint pos, QString src_file, QString dst_file)
{
	cv::Mat img1 = imread(src_file.toStdString(),1);

	cv::Mat img2 = imread(dst_file.toStdString(),1);
	cv::Mat result;

	Mat img3 = img1( Rect( pos.x(),pos.y(), img2.cols,img2.rows));

	cv::compare(img2 , img3  , result , cv::CMP_EQ );
	int percentage  = countNonZero(result);

	return percentage;
}

//最简单的以灰度直方图作为相似比较的实现
float classify_gray_hist(QString src_file, QString dst_file)
{
	Mat m1 = imread(src_file.toStdString(),IMREAD_GRAYSCALE);
	Mat m2 = imread(dst_file.toStdString(),IMREAD_GRAYSCALE);
	return classify_gray_hist(m1,m2);
}
float classify_gray_hist(Mat &m1, Mat &m2)
{
	Size s(256,256);
	Mat mo1, mo2;
	cv::resize(m1, mo1,s,0,0,0);
	cv::resize(m2, mo2,s,0,0,0);

	// Establish the number of bins
	int histSize = 256;
	// Set the range
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;

	Mat hist1;
	// Compute the histogram
	calcHist(&mo1, 1, 0, Mat(), hist1, 1, &histSize, &histRange, uniform, accumulate);
	Mat hist2;
	// Compute the histogram
	calcHist(&mo2, 1, 0, Mat(), hist2, 1, &histSize, &histRange, uniform, accumulate);

	float total = 0;
	for(int i=0; i < hist1.rows; i++)
	{
		if( abs (hist1.at<float>(i) - hist2.at<float>(i)) > 0.00004 )
		{
			total += (1 - abs(hist1.at<float>(i)-hist2.at<float>(i))/max(hist1.at<float>(i),hist2.at<float>(i)) ) ;
		}
		else
			total +=1;
	}
	qDebug() << "rows = " << hist1.rows << hist1.cols;
	//hist1.size().height;
	return total / 256;

}

//平均哈希算法计算
float classify_aHash(QString src_file, QString dst_file)
{
	Mat m1 = imread(src_file.toStdString(),IMREAD_GRAYSCALE);
	Mat m2 = imread(dst_file.toStdString(),IMREAD_GRAYSCALE);
	return classify_aHash(m1,m2);
}

float classify_aHash(Mat m1,Mat m2)
{
	Size s(256,256);
	Mat mo1, mo2;
	cv::resize(m1, mo1,s,0,0,0);
	cv::resize(m2, mo2,s,0,0,0);
	auto r1 = cv::mean(mo1,Mat());
	auto r2 = cv::mean(mo2,Mat());

	auto f= createHausdorffDistanceExtractor();
	f->computeDistance(r1,r2);
}


float hisMatch(QString src_file, QString dst_file)
{

	//read 2 images for histogram comparing
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mat imgA, imgB;
	imgA = imread(src_file.toStdString());
	imgB = imread(dst_file.toStdString());
qDebug() << src_file <<dst_file;
	//imshow("img1", imgA);
	//imshow("img2", imgB);
	if( imgA.data == nullptr || imgB.data == nullptr)
	{
		qDebug() << "image error" << (imgA.data == nullptr) << (imgB.data == nullptr);
	}


	//variables preparing
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int hbins = 30, sbins = 32;
	int channels[] = {0,  1};
	int histSize[] = {hbins, sbins};
	float hranges[] = { 0, 180 };
	float sranges[] = { 0, 255 };
	const float* ranges[] = { hranges, sranges};

	Mat patch_HSV;
	MatND HistA, HistB;

	//cal histogram & normalization
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cvtColor(imgA, patch_HSV, CV_BGR2HSV);
	calcHist( &patch_HSV, 1, channels,  Mat(), // do not use mask
			  HistA, 2, histSize, ranges,
			  true, // the histogram is uniform
			  false );
	normalize(HistA, HistA,  0, 255, CV_MINMAX);

	qDebug() << "cvtcolot 2";

	cvtColor(imgB, patch_HSV, CV_BGR2HSV);
	qDebug() << "calcHist 2";
	calcHist( &patch_HSV, 1, channels,  Mat(),// do not use mask
			  HistB, 2, histSize, ranges,
			  true, // the histogram is uniform
			  false );
	normalize(HistB, HistB, 0, 255, CV_MINMAX);


	//compare histogram
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
qDebug() << "Start compare ...";
	float bc = compareHist(HistA, HistB, CV_COMP_BHATTACHARYYA);
	printf("(The range of matcing value is 0~1, 0 is best matching, 1 means miss matching\n");
	printf("V = %lf \n", bc);


	return bc;
}


void histogram(IplImage* src, int color_histogram[], int N)
{
	int index;
	int width=src->width;
	int height=src->height;
	for (int w=0;w<width;w++)
	{
		for (int h=0;h<height;h++)
		{
			int Blue = ((uchar *)(src->imageData + h*src->widthStep))[w*src->nChannels +0];
			int Green = ((uchar *)(src->imageData + h*src->widthStep))[w*src->nChannels +1];
			int Red = ((uchar *)(src->imageData + h*src->widthStep))[w*src->nChannels +2];
			index = (Blue/64)*16+(Green/64)*4+(Red/64)*1;
			++color_histogram[index];
		}
	}
}
double math_coefficient(int a[], int b[], int N)
{
	double Sum_xy = 0;
	double Sum_x =0;
	double Sum_y = 0;
	double Sum_x2 = 0;
	double Sum_y2 = 0;
	for(int i=0; i<N; i++)
	{

		Sum_xy += a[i]*b[i];
		Sum_x += a[i];
		Sum_y += b[i];
		Sum_x2 += a[i]*a[i];
		Sum_y2 += b[i]*b[i];
	}

	return (Sum_xy*N-Sum_x*Sum_y)/sqrt((N*Sum_x2-pow(Sum_x,2))*(N*Sum_y2-pow(Sum_y,2)));
}
double image_coefficient(IplImage* src1, IplImage* src2)
{
	int H1[64] = {0};
	int H2[64] = {0};
	histogram(src1, H1,64);
	histogram(src2, H2,64);

	return math_coefficient(H1,H2,64);
}
double image_coefficient(QString src_file, QString dst_file)
{
	IplImage* src1 = cvLoadImage(src_file.toUtf8(),1);
	IplImage* src2 = cvLoadImage(dst_file.toUtf8(),1);
	return image_coefficient(src1,src2);

}

int MatchPoint(QString src_file, QString dst_file)
{
	Mat img_1 = imread( src_file.toStdString(), IMREAD_GRAYSCALE );
	Mat img_2 = imread( dst_file.toStdString(), IMREAD_GRAYSCALE );

	 if( !img_1.data || !img_2.data )
	   { std::cout<< " --(!) Error reading images " << std::endl; return -1; }
	   //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
	   int minHessian = 400;
	   Ptr<SURF> detector = SURF::create();
	   detector->setHessianThreshold(minHessian);
	   std::vector<KeyPoint> keypoints_1, keypoints_2;
	   Mat descriptors_1, descriptors_2;
	   detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
	   detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );
	   //-- Step 2: Matching descriptor vectors using FLANN matcher
	   FlannBasedMatcher matcher;
	   std::vector< DMatch > matches;
	   matcher.match( descriptors_1, descriptors_2, matches );
	   double max_dist = 0; double min_dist = 100;
	   //-- Quick calculation of max and min distances between keypoints
	   for( int i = 0; i < descriptors_1.rows; i++ )
	   { double dist = matches[i].distance;
		 if( dist < min_dist ) min_dist = dist;
		 if( dist > max_dist ) max_dist = dist;
	   }
	   printf("-- Max dist : %f \n", max_dist );
	   printf("-- Min dist : %f \n", min_dist );
	   //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	   //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	   //-- small)
	   //-- PS.- radiusMatch can also be used here.
	   std::vector< DMatch > good_matches;
	   for( int i = 0; i < descriptors_1.rows; i++ )
	   { if( matches[i].distance <= max(2*min_dist, 0.02) )
		 { good_matches.push_back( matches[i]); }
	   }
	   //-- Draw only "good" matches
	   Mat img_matches;
	   drawMatches( img_1, keypoints_1, img_2, keypoints_2,
					good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
					vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
	   //-- Show detected matches
	   imshow( "Good Matches", img_matches );
	   for( int i = 0; i < (int)good_matches.size(); i++ )
	   {
		   printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx );
		   cout << keypoints_1[i].pt.x << ", " << keypoints_1[i].pt.y << endl;
	   }
	 waitKey(0);

		return 0;
}



Mat img; Mat templ; Mat result;
char* image_window = "Source Image"; //窗口名称定义
char* result_window = "Result window";  //窗口名称定义


int match_method;
int max_Trackbar = 5;




void MatchingMethod( int, void* )
{

  Mat img_display;
  img.copyTo( img_display ); //将 img的内容拷贝到 img_display

  /// 创建输出结果的矩阵
  int result_cols =  img.cols - templ.cols + 1;     //计算用于储存匹配结果的输出图像矩阵的大小。
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_cols, result_rows, CV_32FC1 );//被创建矩阵的列数，行数，以CV_32FC1形式储存。

  /// 进行匹配和标准化
  matchTemplate( img, templ, result, match_method ); //待匹配图像，模版图像，输出结果图像，匹配方法（由滑块数值给定。）
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );//输入数组，输出数组，range normalize的最小值，range normalize的最大值，归一化类型，当type为负数的时候输出的type和输入的type相同。

  /// 通过函数 minMaxLoc 定位最匹配的位置
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );//用于检测矩阵中最大值和最小值的位置

  /// 对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值代表更高的匹配结果. 而对于其他方法, 数值越大匹配越好
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
	{ matchLoc = minLoc; }
  else
	{ matchLoc = maxLoc; }

  /// 让我看看您的最终结果
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0,0,255), 2, 8, 0 ); //将得到的结果用矩形框起来
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0,0,255), 2, 8, 0 );

  imshow( image_window, img_display );//输出最终的到的结果
  imwrite("result.jpg",img_display); //将得到的结果写到源代码目录下。
  imshow( result_window, result );   //输出匹配结果矩阵。

  return;
}


int CallMatchingMethod( )
{

  img = imread("/rong/1.jpg");//载入待匹配图像
  templ = imread("/rong/2.jpeg");//载入模版图像

  /// 创建窗口
  namedWindow( image_window, CV_WINDOW_AUTOSIZE ); // 窗口名称，窗口标识CV_WINDOW_AUTOSIZE是自动调整窗口大小以适应图片尺寸。
  namedWindow( result_window, CV_WINDOW_AUTOSIZE );

  /// 创建滑动条
  createTrackbar("jackchen", image_window, &match_method, max_Trackbar, MatchingMethod ); //滑动条提示信息，滑动条所在窗口名，匹配方式（滑块移动之后将移动到的值赋予该变量），回调函数。

  MatchingMethod( 0, 0 );//初始化显示

  int logo = waitKey(5000); //等待按键事件，如果值0为则永久等待。

  return 0;
}



/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */



QPoint Match2(QString src_file, QString dst_file, int md)
{
	Mat srcImg =  imread(src_file.toStdString(),1);
	Mat tempImg = imread(dst_file.toStdString(),1);
	if(srcImg.data == nullptr || tempImg.data==nullptr )
	{
		return  QPoint();
	}

	cout << "Size of template: "<<tempImg.size() << endl;
	//1.构建结果图像resultImg(注意大小和类型)
	//如果原图(待搜索图像)尺寸为W x H, 而模版尺寸为 w x h, 则结果图像尺寸一定是(W-w+1)x(H-h+1)
	//结果图像必须为单通道32位浮点型图像
	int width = srcImg.cols - tempImg.cols + 1;
	int height = srcImg.rows - tempImg.rows + 1;
	Mat resultImg(Size(width,height),CV_32FC1);
	//2.模版匹配
	matchTemplate(srcImg, tempImg, resultImg, md);

	//3.正则化(归一化到0-1)
	normalize(resultImg,resultImg,0,1,NORM_MINMAX,-1);
	//4.找出resultImg中的最大值及其位置
	double minValue = 0;
	double maxValue = 0;
	Point minPosition;
	Point maxPosition;
	minMaxLoc(resultImg,&minValue,&maxValue,&minPosition,&maxPosition);
	cout << "minValue: " << minValue << endl;
	cout << "maxValue: " << maxValue << endl;
	cout << "minPosition: " << minPosition << endl;
	cout << "maxPosition: " << maxPosition << endl;
	//5.根据resultImg中的最大值位置在源图上画出矩形
	rectangle(srcImg,maxPosition,Point(maxPosition.x+tempImg.cols,maxPosition.y+tempImg.rows),Scalar(0,255,0),1,8);
	//imshow("result",resultImg);
	return QPoint(maxPosition.x,maxPosition.y);
}






double getPSNR ( const Mat& I1, const Mat& I2);
Scalar getMSSIM( const Mat& I1, const Mat& I2);

int diff(QString src_file, QString dst_file)
{
	Mat frameReference = imread( src_file.toStdString(),0);
	Mat frameUnderTest = imread( dst_file.toStdString(),0);;
	double psnrV;
	Scalar mssimV;

	///////////////////////////////// PSNR ////////////////////////////////////////////////////
	psnrV = getPSNR(frameReference,frameUnderTest);
	qDebug() <<   psnrV << "dB";

	//////////////////////////////////// MSSIM /////////////////////////////////////////////////
	if (psnrV < 40 && psnrV)
	{
		mssimV = getMSSIM(frameReference, frameUnderTest);

		qDebug() << " MSSIM: "
			<< " R " <<  mssimV.val[2] * 100 << "%"
			<< " G " <<  mssimV.val[1] * 100 << "%"
			<< " B " <<  mssimV.val[0] * 100 << "%";
	}

	return 0;
}

double getPSNR(const Mat& I1, const Mat& I2)
{
	Mat s1;
	absdiff(I1, I2, s1);       // |I1 - I2|
	s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
	s1 = s1.mul(s1);           // |I1 - I2|^2

	Scalar s = sum(s1);        // sum elements per channel

	double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

	if( sse <= 1e-10) // for small values return zero
		return 0;
	else
	{
		double mse  = sse / (double)(I1.channels() * I1.total());
		double psnr = 10.0 * log10((255 * 255) / mse);
		return psnr;
	}
}

Scalar getMSSIM( const Mat& i1, const Mat& i2)
{
	const double C1 = 6.5025, C2 = 58.5225;
	/***************************** INITS **********************************/
	int d = CV_32F;

	Mat I1, I2;
	i1.convertTo(I1, d);            // cannot calculate on one byte large values
	i2.convertTo(I2, d);

	Mat I2_2   = I2.mul(I2);        // I2^2
	Mat I1_2   = I1.mul(I1);        // I1^2
	Mat I1_I2  = I1.mul(I2);        // I1 * I2

	/*************************** END INITS **********************************/

	Mat mu1, mu2;                   // PRELIMINARY COMPUTING
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);

	Mat mu1_2   =   mu1.mul(mu1);
	Mat mu2_2   =   mu2.mul(mu2);
	Mat mu1_mu2 =   mu1.mul(mu2);

	Mat sigma1_2, sigma2_2, sigma12;

	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;

	GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;

	///////////////////////////////// FORMULA ////////////////////////////////
	Mat t1, t2, t3;

	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

	Mat ssim_map;
	divide(t3, t1, ssim_map);        // ssim_map =  t3./t1;

	Scalar mssim = mean(ssim_map);   // mssim = average of ssim map
	return mssim;
}

