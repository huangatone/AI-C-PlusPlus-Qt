#include "tourist.h"
#include <QFile>
#include <QDebug>

// 声明 原始图片，灰度图片，和 canny边缘图片
	Mat image, cedge;
	Mat gray, edge;

Tourist::Tourist()
{

}

Mat  Tourist::addImage( QString f1, QString f2,double a, double b )
{
   double alpha = 0.5; double beta; double input;
   Mat src1, src2, dst;

   src1 = imread( f1.toStdString() );
   src2 = imread( f2.toStdString() );
   if( src1.empty() ) { qDebug() << "Error loading src1" << f1; return Mat(); }
   if( src2.empty() ) { qDebug() << "Error loading src2" << f2; return Mat(); }

   int col = qMin( src1.cols, src2.cols);
   int row = qMin( src1.rows, src2.rows);

   Mat m1 = src1(Range(0,row), Range(0,col));
   Mat m2 = src2(Range(0,row), Range(0,col));
   beta = ( 1.0 - alpha );
   addWeighted( m1, a, m2, b, 0.0, dst);
   //imshow( "Linear Blend", dst );
   //imwrite("/rong/testfile/r1.png", src1);
   //imwrite("/rong/testfile/r2.png", src2);
   //waitKey(0);
   //RNG rng(12345);
   //cv::Mat imageROI= dst(cv::Rect(110,260,35,40));
   //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
   //rectangle( imageROI, Point(2,2), Point(20,20), color, 2, 8, 0 );
   //circle(  imageROI, center[i], (int)radius[i], color, 2, 8, 0 );


   return dst;
}

Mat Tourist::onCandy(QString f_n,int iThresh)
{
	// 载入图片
	image = imread(f_n.toStdString(), 1);
	// 判断载入图片是否成功
	if(image.empty())
	{
		printf("miss the image file: %d \n","s_orig");
		return Mat();
	}

	// 生成灰度图片，因为只有灰度图片才能生成边缘图片
	cedge.create(image.size(), image.type());
	cvtColor(image,gray, CV_BGR2GRAY);
	//blur 灰度图片
	blur(gray, edge, Size(3,3));

	// Canny 边缘检测
	Canny(gray,edge, iThresh, iThresh*3, 3);

	//全部设为0
	cedge = Scalar::all(0);

	//拷贝边缘的象素点
	image.copyTo(cedge, edge);

	return edge;
}

Mat Tourist::onChangeThresh(int iThresh)
{
	// Canny 边缘检测
	Canny(gray,edge, iThresh, iThresh*3, 3);

	//全部设为0
	cedge = Scalar::all(0);

	//拷贝边缘的象素点
	image.copyTo(cedge, edge);

	return edge;
}


Mat Tourist::random_kmeans( )
{
	const int MAX_CLUSTERS = 5;
	Scalar colorTab[] =
	{
		Scalar(0, 0, 255),
		Scalar(0,255,0),
		Scalar(255,100,100),
		Scalar(255,0,255),
		Scalar(0,255,255)
	};

	Mat img(500, 500, CV_8UC3);
	RNG rng(12345);

	//for(;;)
	{
		int k, clusterCount = rng.uniform(2, MAX_CLUSTERS+1);
		int i, sampleCount = rng.uniform(1, 1001);
		Mat points(sampleCount, 1, CV_32FC2), labels;

		clusterCount = MIN(clusterCount, sampleCount);
		std::vector<Point2f> centers;

		/* generate random sample from multigaussian distribution */
		for( k = 0; k < clusterCount; k++ )
		{
			Point center;
			center.x = rng.uniform(0, img.cols);
			center.y = rng.uniform(0, img.rows);
			Mat pointChunk = points.rowRange(k*sampleCount/clusterCount,
											 k == clusterCount - 1 ? sampleCount :
											 (k+1)*sampleCount/clusterCount);
			rng.fill(pointChunk, RNG::NORMAL, Scalar(center.x, center.y), Scalar(img.cols*0.05, img.rows*0.05));
		}

		randShuffle(points, 1, &rng);

		double compactness = kmeans(points, clusterCount, labels,
			TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
			   3, KMEANS_PP_CENTERS, centers);

		img = Scalar::all(0);

		for( i = 0; i < sampleCount; i++ )
		{
			int clusterIdx = labels.at<int>(i);
			Point ipt = points.at<Point2f>(i);
			circle( img, ipt, 2, colorTab[clusterIdx], FILLED, LINE_AA );
		}
		for (i = 0; i < (int)centers.size(); ++i)
		{
			Point2f c = centers[i];
			circle( img, c, 40, colorTab[i], 1, LINE_AA );
		}
		cout << "Compactness: " << compactness << endl;

		//imshow("clusters", img);

		//char key = (char)waitKey();
		//if( key == 27 || key == 'q' || key == 'Q' ) // 'ESC'
		//	;
	}

	return img;
}

Mat Tourist::segmentation()
{
	Mat img0 = imread("/rong/tmp/clock_tower.jpg",1);

	Mat img,fgimg,fgmask;

	resize(img0, img, Size(640, 640*img0.rows/img0.cols), 0, 0, INTER_LINEAR_EXACT);

	if( fgimg.empty() )
	  fgimg.create(img.size(), img.type());

	Ptr<BackgroundSubtractor> bg_model =
				//createBackgroundSubtractorKNN().dynamicCast<BackgroundSubtractor>() ;
				createBackgroundSubtractorMOG2().dynamicCast<BackgroundSubtractor>();

	//update the model
	bg_model->apply(img, fgmask,  0);
	//if( smoothMask )
	{
		GaussianBlur(fgmask, fgmask, Size(11, 11), 3.5, 3.5);
		threshold(fgmask, fgmask, 10, 255, THRESH_BINARY);
	}

	fgimg = Scalar::all(0);
	img.copyTo(fgimg, fgmask);

	Mat bgimg;
	bg_model->getBackgroundImage(bgimg);

	imshow("image", img);
	imshow("foreground mask", fgmask);
	imshow("foreground image", fgimg);
	if(!bgimg.empty())
	  imshow("mean background image", bgimg );
}

Mat Tourist::Seamless_Clone(QString src1, QString dst1, QString m)
{


	// Read images : src image will be cloned into dst
	Mat src = imread(src1.toStdString());
	Mat dst = imread(dst1.toStdString());
	Mat src_mask;
	if(m == "")
	{
		// Create a rough mask around the airplane.
		src_mask = 255 * Mat::ones( src.cols,src.rows, src.depth());

		// Define the mask as a closed polygon
		Point poly[1][7];
		poly[0][0] = Point(4, 80);
		poly[0][1] = Point(30, 54);
		poly[0][2] = Point(151,63);
		poly[0][3] = Point(254,37);
		poly[0][4] = Point(298,90);
		poly[0][5] = Point(272,134);
		poly[0][6] = Point(43,122);

		const Point* polygons[1] = { poly[0] };
		int num_points[] = { 7 };

		// Create mask by filling the polygon

		fillPoly(src_mask, polygons, num_points, 1, Scalar(255,255,255));
	}else
	{
		src_mask = imread(m.toStdString());
	}

	// The location of the center of the src in the dst
	Point center(dst.cols/2,dst.rows/2);




	// Seamlessly clone src into dst and put the results in output

	qDebug() << "size = " << src.cols << src.rows << dst.cols << dst.rows;
	Mat output;
	seamlessClone(src, dst, src_mask, center, output, NORMAL_CLONE);

	// Save result
	imwrite("/rong/testfile/opencv-seamless-cloning-example.jpg", output);
	return output;
}


Mat Tourist::Seamless_Clone(QString src1, QString dst1, int n)
{
	// Read images : src image will be cloned into dst
	Mat src = imread(src1.toStdString());
	Mat dst = imread(dst1.toStdString());
	Mat src_mask;

	// Create a rough mask around the airplane.
	src_mask = 255 * Mat::ones( src.cols,src.rows, src.depth());

	// Define the mask as a closed polygon
	Point poly[1][7];
	poly[0][0] = Point(4, 80);
	poly[0][1] = Point(30, 54);
	poly[0][2] = Point(151,63);
	poly[0][3] = Point(254,37);
	poly[0][4] = Point(298,90);
	poly[0][5] = Point(272,134);
	poly[0][6] = Point(43,122);

	const Point* polygons[1] = { poly[0] };
	int num_points[] = { 7 };

	// Create mask by filling the polygon

	fillPoly(src_mask, polygons, num_points, 1, Scalar(n/100.00 *255,n/100.00 *255,n/100.00 *255));


	// The location of the center of the src in the dst
	Point center(dst.cols/2,dst.rows/2);

	// Seamlessly clone src into dst and put the results in output
	Mat output;
	seamlessClone(src, dst, src_mask, center, output, NORMAL_CLONE);

	// Save result
	imwrite("/rong/testfile/opencv-seamless-cloning-example.jpg", output);
	return output;
}

/*
void Tourist::onCandyTrackbar(int, void *p)
{
	//blur 灰度图片
		blur(gray, edge, Size(3,3));

		// Canny 边缘检测
		Canny(gray,edge, edgeThresh, edgeThresh*3, 3);

		//全部设为0
		cedge = Scalar::all(0);

		//拷贝边缘的象素点
		image.copyTo(cedge, edge);

		imshow("Edge map", edge);

}*/

/*

	Mat source = imread(src.toStdString());
	Mat destination = imread(dst.toStdString());
	Mat mask = imread(m.toStdString());

	if(source.empty())
	{
		qDebug() << "Could not load source image " << src ;
		return Mat();
	}
	if(destination.empty())
	{
		qDebug()  << "Could not load destination image " << dst ;
		return Mat();
	}
	if(mask.empty())
	{
		qDebug()  << "Could not load mask image " << m ;
		return Mat();
	}

	Mat result;
	Point p;
	p.x = 300;
	p.y = 100;

	qDebug() << "do seamlessClone";
	seamlessClone(source, destination, mask, p, result, 1);

	imshow("Output",result);
	imwrite("cloned.png", result);
*/
/*
int Tourist::doCanny()
{
	// 载入图片
	image = imread("/rong/tmp/s_orig.png", 1);

	// 判断载入图片是否成功
	if(image.empty())
	{
		printf("miss the image file: %d \n","s_orig");
		return -1;
	}

	// 生成灰度图片，因为只有灰度图片才能生成边缘图片
	cedge.create(image.size(), image.type());
	cvtColor(image,gray, CV_BGR2GRAY);

	//新建一个窗口
	namedWindow("Edge map", 1);

	// 生成一个进度条来控制边缘检测
	createTrackbar("Canny Threshold", "Edge map", &edgeThresh, 100, onCandyTrackbar,this);

	//初始化图像
	onCandyTrackbar(0,0);

	waitKey(0);

	return 0;
}
*/
