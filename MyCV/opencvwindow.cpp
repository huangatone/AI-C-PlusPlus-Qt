#include "opencvwindow.h"
#include "ui_opencvwindow.h"

#include <QSplitter>
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
#include <QPainter>

#include <QLinearGradient>
#include "opencv2/text.hpp"
#include "dfsplitter.h"



OpenCVWindow::OpenCVWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::OpenCVWindow)
{
	ui->setupUi(this);

	cout << "d";

	auto sp1 = new DFSplitter(Qt::Horizontal,this);
	//auto sp1 = new QSplitter(Qt::Horizontal,this);
	setCentralWidget(sp1);
	mainSplitter = sp1;
	mainSplitter->setHandleWidth(12);
	mainSplitter->setCollapsible(0,false);
	mainSplitter->setCollapsible(1,false);

	//connect(mainSplitter, SIGNAL(splitterMoved(int , int )), this , SLOT(slotMainSplitterMoved()));

	auto subSp = new DFSplitter(Qt::Vertical,this);
	subSp->setHandleWidth(12);

	sp1->addWidget(subSp);
	sp1->addWidget(ui->scrollArea);

	auto p1 = new DFPixmapWdiget(this);
	auto p2 = new DFPixmapWdiget(this);

	subSp->addWidget(p1);
	subSp->addWidget(p2);

	_pixWidget1 = p1;
	_pixWidget2 = p2;
	//load ini file
	QFile ini( "/rong/lib/cv.ini");
	ini.open(QIODevice::ReadOnly);
	QTextStream iniS(&ini);

	auto all = iniS.readAll();
	auto lt = all.split("\n");

	ui->comboBox->addItems(lt);

	// Prepare arguments for 1D histogram
	histSize[0]= 256;
	hranges[0]= 0.0;
	hranges[1]= 255.0;
	ranges[0]= hranges;
	channels[0]= 0; // by default, we look at channel 0
	_sec_size = 150;

	QSplitterHandle *p = 	mainSplitter->handle(1) ;
	//connect(p, SIGNAL(doubleClicked()), this, SLOT(handleDoubleClicked()));
	p->setMouseTracking(true);
	//if(p)
	//	p->installEventFilter(this);

	ui->pp3->installEventFilter(this);

	Mat image= cv::imread( "/rong/tmp/abc.png",CV_BGR2GRAY);

	this->myCV.polygonfind( image );
}

OpenCVWindow::~OpenCVWindow()
{
	delete ui;
}

bool OpenCVWindow::eventFilter(QObject *watched, QEvent *event)
{

	return false;
}

void OpenCVWindow::showEvent(QShowEvent *event)
{
	setGeometry(0,0,800,600);
}
void OpenCVWindow::resizeEvent(QResizeEvent *event)
{
	int w = width() - ui->dockWidget->width();
	auto sz = mainSplitter->sizes();
	qDebug() <<sz;
	QList<int > nSZ;

	if(w >_sec_size)
		nSZ << w - _sec_size << _sec_size;
	else
		nSZ <<(w - w* 0.3)<< (w*0.3);
	qDebug() << " ==" << nSZ << w << sz[1];
	mainSplitter->setSizes(nSZ);
}


void OpenCVWindow::on_btnProcess_clicked()
{
	auto cmd = ui->comboBox->currentText();

	Mat image= cv::imread( _pixWidget1->getFileNmae().toStdString());
	if(image.data == nullptr)
		return;

	if(cmd == "Histograms")
	{
		Mat result = myCV.HistogramRGB( _pixWidget1->getFileNmae());
		auto img = myCV.Mat2QImage(result);
		_pixWidget2->setImage(img);
	}
	else if(cmd == "ROI")
	{
		Mat result = myCV.HistogramRGB( _pixWidget1->getFileNmae());
		if (!image.data)
			return ;

		// Define ROI
		cv::Mat imageROI= image(cv::Rect(110,260,35,40));
		cv::rectangle(image, cv::Rect(110,260,35,40), cv::Scalar(0,0,255));

		// Display image
		auto img = myCV.Mat2QImage(image);
		_pixWidget2->setImage(img);
	}
	else if(cmd == "ROI2")
	{
		if (!image.data)
			return ;
		Mat result = myCV.HistogramRGB( _pixWidget1->getFileNmae());

		// Select ROI
		Rect2d r = selectROI(image);
		cv::waitKey(0);
		cv::destroyAllWindows();

		// Crop image
		Mat imCrop = image(r);
		// Display image
		auto img = myCV.Mat2QImage(imCrop);
		_pixWidget2->setImage(img);
	}
	else if(cmd.startsWith("Salt",Qt::CaseInsensitive))
	{

		Mat result = image;
		int n = image.rows * image.cols /100;
		myCV.salt(result,n);
		onDisplayResult(result);
	}
	else if(cmd.startsWith("Reduce",Qt::CaseInsensitive))
	{
		Mat result = image;
		myCV.colorReduce0(result,80);
		QImage 	qimg = myCV.Mat2QImage(result);
		onDisplayResult(qimg);
	}
	else if(cmd.startsWith("Channel",Qt::CaseInsensitive))
		onChannel(image);
	else if(cmd.startsWith("Logo",Qt::CaseInsensitive))
		onLogo(image);
	else if(cmd.startsWith("flip",Qt::CaseInsensitive))
		onFlip(image);
	else if(cmd.startsWith("add",Qt::CaseInsensitive))
		onAdd(image);
	else if(cmd.startsWith("2Logo",Qt::CaseInsensitive))
		on2Logo(image);
	else if(cmd.startsWith("color",Qt::CaseInsensitive))
		onColor(130,190,230);
	else if(cmd.startsWith("detect",Qt::CaseInsensitive))	
		onDetect(image);
	else if(cmd == "histogram1d")
		onHistogram1d(image);
	else if(cmd == "morpho")
		// Create the morphological features instance
		  onMorpho(image);
	else if(cmd == "sobelX")
	{
		// Read input image

		// Compute Sobel X derivative
		cv::Mat sobelX;
		cv::Sobel(image,sobelX,CV_8U,1,0,3,0.4,128);

		// Display the image
		onDisplayResult(sobelX);
	}
	else if(cmd == "sobleY")
	{

		// Compute Sobel Y derivative
		cv::Mat sobelY;
		cv::Sobel(image,sobelY,CV_8U,0,1,3,0.4,128);

		// Display the image
		onDisplayResult(sobelY);
	}
	else if(cmd == "sobelXY")
	{
		// Compute Sobel X derivative
		cv::Mat sobelX;
		cv::Sobel(image,sobelX,CV_8U,1,0,3,0.4,128);
		// Compute Sobel Y derivative
		cv::Mat sobelY;
		cv::Sobel(image,sobelY,CV_8U,0,1,3,0.4,128);
		// Compute norm of Sobel
		cv::Sobel(image,sobelX,CV_16S,1,0);
		cv::Sobel(image,sobelY,CV_16S,0,1);
		cv::Mat sobel;
		//compute the L1 norm
		sobel= abs(sobelX)+abs(sobelY);

		double sobmin, sobmax;
		cv::minMaxLoc(sobel,&sobmin,&sobmax);
		std::cout << "sobel value range: " << sobmin << "  " << sobmax << std::endl;

		// Print window pixel values
		/*for (int i=0; i<12; i++) {
			for (int j=0; j<12; j++)
				std::cout << std::setw(5) << static_cast<int>(sobel.at<short>(i+135,j+362)) << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;*/

		// Conversion to 8-bit image
		// sobelImage = -alpha*sobel + 255
		cv::Mat sobelImage;
		sobel.convertTo(sobelImage,CV_8U,-255./sobmax,255);

		onDisplayResult(sobelX, ui->pp1);
		onDisplayResult(sobelY,ui->pp2);
		onDisplayResult( sobelImage,ui->pp3);

		// Apply threshold to Sobel norm (low threshold value)
		cv::Mat sobelThresholded;
		cv::threshold(sobelImage, sobelThresholded, 225, 255, cv::THRESH_BINARY);

		// Display the image

		onDisplayResult(sobelThresholded, ui->pp4);

		// Apply threshold to Sobel norm (high threshold value)
		cv::threshold(sobelImage, sobelThresholded, 190, 255, cv::THRESH_BINARY);

		// Display the image
		onDisplayResult(sobelThresholded);

		// Compute Laplacian 3x3
	}
	else if( cmd == "laplace")
	{
		cv::Mat laplace;
		cv::Laplacian(image,laplace,CV_8U,1,1,128);

		// Display the image
		onDisplayResult( laplace, ui->pp1);

		// Print window pixel values
		/*for (int i=0; i<12; i++) {
			for (int j=0; j<12; j++)
				std::cout << std::setw(5) << static_cast<int>(laplace.at<uchar>(i+135,j+362))-128 << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;*/

		// Compute Laplacian 7x7
		cv::Laplacian(image,laplace,CV_8U,7,0.01,128);

		// Display the image

		onDisplayResult( laplace,ui->pp2);

		// Print window pixel values
		for (int i=0; i<12; i++) {
			for (int j=0; j<12; j++)
				std::cout << std::setw(5) << static_cast<int>(laplace.at<uchar>(i+135,j+362))-128 << " ";
			std::cout << std::endl;
		}

		// Extract small window


		// Compute Laplacian using LaplacianZC class
		LaplacianZC laplacian;
		laplacian.setAperture(7);
		cv::Mat flap= laplacian.computeLaplacian(image);
		double lapmin, lapmax;
		cv::minMaxLoc(flap,&lapmin,&lapmax);
		std::cout << "Laplacian value range=[" << lapmin << "," << lapmax << "]\n";
		laplace= laplacian.getLaplacianImage();
		cv::namedWindow("Laplacian Image (7x7)");
		onDisplayResult( laplace , ui->pp3);

		// Print Laplacian values
		std::cout << std::endl;
		for (int i=0; i<12; i++) {
			for (int j=0; j<12; j++)
				std::cout << std::setw(5) << static_cast<int>(flap.at<float>(i+135,j+362)/100) << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;

		// Compute and display the zero-crossing points
		cv::Mat zeros;
		zeros= laplacian.getZeroCrossings(lapmax);
		onDisplayResult( zeros, ui->pp4);

		// Compute and display the zero-crossing points (Sobel version)
		zeros= laplacian.getZeroCrossings();
		zeros= laplacian.getZeroCrossingsWithSobel(50);
		onDisplayResult( zeros, ui->pp4);

		// Print window pixel values
		for (int i=0; i<12; i++) {
			for (int j=0; j<12; j++)
				std::cout << std::setw(2) << static_cast<int>(zeros.at<uchar>(i+135,j+362)) << " ";
			std::cout << std::endl;
		}

		// Display the image with window
		 onDisplayResult( image);

	}
	else if(cmd == "onDetectAndDisplay")
	{
		onDetectAndDisplay(image);
	}
	else if(cmd == "sharp")
		onSharp(image);
	else if(cmd == "GetTextFromPicture")
		;//GetTextFromPicture(image);
	else if(cmd == "text_detect")
		myCV.text_detect2(image);
	// cv::Mat thresholded;
	//cv::threshold(image,thresholded,60,255,cv::THRESH_BINARY);
}


//OK
void OpenCVWindow::onSalt()
{
}

//OK
void OpenCVWindow::onReduce()
{

}

void OpenCVWindow::onChannel(Mat &img)
{
	cv::Mat image1;
	cv::Mat image2;
	cv::Mat result;

	image1= img;
	//MYMEMO: second peremeter 0
	image2= cv::imread(getFileName2() ,0);

	if (!image1.data || !image2.data)
		return;
	if( image1.rows != image2.rows || image1.cols != image2.cols)
	{
		QMessageBox::warning(this,"Size not match","please using the same size image");
		return;
	}
	// create vector of 3 images
	std::vector<cv::Mat> planes;
	// split 1 3-channel image into 3 1-channel images
	cv::split(image1,planes);
	// add to blue channel
	planes[0]+= image2;
	// merge the 3 1-channel images into 1 3-channel image
	cv::merge(planes,result);
	QImage 	qimg = myCV.Mat2QImage(result);
	onDisplayResult(qimg);
}

void OpenCVWindow::on2Logo(Mat &img)
{
	// read images
	qDebug() << "logo 2";
	cv::Mat	logo= cv::imread(getFileName2(),0);
	cv::Mat	image1= img;
	if(!logo.data || !image1.data)
		return;

	// split 3-channel image into 3 1-channel images
	std::vector<cv::Mat> channels;
	cv::split(image1,channels);

	if( image1.rows < logo.rows || image1.cols < logo.cols)
	{
		QMessageBox::warning(this,"Size not match","logo size must smaller than image");
		return;
	}

	cv::Mat	imageROI= channels.at(1);
	if(imageROI.data == nullptr)
		return;
	qDebug() << "do --1";
	qDebug() <<"size = " <<  imageROI.size().width << imageROI.size().height;

	cv::Mat m1 = imageROI(cv::Rect(image1.cols -logo.cols ,image1.rows - logo.rows,logo.cols,logo.rows));

	qDebug() <<"size = " <<  m1.size().width << m1.size().height;
	qDebug() << logo.size().width << logo.size().height;

	cv::addWeighted( m1, 1.0,	logo,0.5,0.,m1);

	cv::merge(channels,image1);

	QImage 	qimg = myCV.Mat2QImage(image1);

	onDisplayResult(qimg);
}
void OpenCVWindow::onLogo(Mat &img)
{
	// define ROI
	Mat logo = cv::imread( getFileName2());
	Mat image1= img;

	if( image1.rows < logo.rows || image1.cols < logo.cols)
	{
		QMessageBox::warning(this,"Size not match","logo size must smaller than image");
		return;
	}

	cv::Mat imageROI= image1(cv::Range(image1.rows - logo.rows,image1.rows),
							  cv::Range(image1.cols - logo.cols,image1.cols));
	// load the mask (must be gray-level)
	cv::Mat mask= cv::imread(getFileName2(),0);
	// copy to ROI with mask
	logo.copyTo(imageROI,mask);

	QImage 	qimg = myCV.Mat2QImage(image1);

	onDisplayResult(qimg);
}
void  OpenCVWindow::onFlip(Mat &image)
{
	if (!image.data) {
			// no image has been created…
			return ;
		}
	// create another image
		cv::Mat result;
		// flip the image
		cv::flip(image,result,1); // positive for horizontal
								  // 0 for vertical,
								  // negative for both

	QImage 	qimg = myCV.Mat2QImage(result);

	onDisplayResult(qimg);
}

void OpenCVWindow::onAdd(Mat &img)
{
	cv::Mat image1;
	cv::Mat image2;

	image1= img;
	image2= cv::imread(getFileName2());
	if (!image1.data)
	return ;
	if (!image2.data)
	return ;

	if( image1.rows != image2.rows || image1.cols != image2.cols)
	{
		QMessageBox::warning(this,"Size not match","The twoo picture should have same size");
		return;
	}


	cv::Mat result;
	cv::addWeighted(image1,0.7,image2,0.9,0.,result);
	// using overloaded operator
	//result= 0.7*image1+0.9*image2;
	QImage 	qimg = myCV.Mat2QImage(result);
	onDisplayResult(qimg);
}

void OpenCVWindow::onColor(uint r, uint g, uint b)
{

	// 1. Create image processor object
	  ColorDetector cdetect;
	  cdetect.setColorDistanceThreshold( ui->threshholdW->value());
	  // 2. Read input image
	  cv::Mat image= cv::imread(getFileName1());
	  if (!image.data)
		  return;
	   // 3. Set input parameters
	   cdetect.setTargetColor( ui->colorW->color().red(),ui->colorW->color().green(),ui->colorW->color().blue()); // here blue sky

	   // 4. Process the image and display the result
	   Mat result = cdetect.process(image);

	   //BGR openCV Mat to QImage
	   QImage img_qt = QImage((const unsigned char*)result.data,result.cols, image.rows, result.step, QImage::Format_RGB888);

	   //For Binary Images
	   if (img_qt.isNull()){
		   //ColorTable for Binary Images
		   QVector<QRgb> colorTable;
		   for (int i = 0; i < 256; i++)
			   colorTable.push_back(qRgb(i, i, i));

		   img_qt = QImage((const unsigned char*)result.data,result.cols, result.rows, QImage::Format_Indexed8);
		   img_qt.setColorTable(colorTable);
		   }

	   //Display the QImage in the Label
	   QPixmap img_pix = QPixmap::fromImage(img_qt.rgbSwapped()); //BGR to RGB
	  _pixWidget2->setImage(img_pix);



/*
	// 1. Create image processor object
   ColorDetector cdetect;
   //cdetect.setColorDistanceThreshold(ui->threshholdW->value());
   r = ui->colorW->color().red();
   g = ui->colorW->color().red();
   b = ui->colorW->color().red();
   // 2. Read input image

   Mat image= cv::imread(getFileName1());
	if (!image.data)
			return ;
	 // 3. Set input parameters
	 cdetect.setTargetColor(130,190,230); // here blue sky

	 // 4. Process the image and display the result
	 cv::Mat  result = cdetect.process(image);


	  QImage img_qt = QImage((const unsigned char*)result.data,result.cols, result.rows, result.step, QImage::Format_RGB888);

	  //For Binary Images
	  if (img_qt.isNull())
	  {
		  //ColorTable for Binary Images
		  QVector<QRgb> colorTable;
		  for (int i = 0; i < 256; i++)
			  colorTable.push_back(qRgb(i, i, i));

		  img_qt = QImage((const unsigned char*)image.data,image.cols, image.rows, QImage::Format_Indexed8);
		  img_qt.setColorTable(colorTable);
	  }

	  //Display the QImage in the Label
	  QImage img = img_qt.rgbSwapped();
	  onDisplayResult(img);

*/
}

string OpenCVWindow::getFileName1()
{
	return _pixWidget1->getFileNmae().toStdString();
}

string OpenCVWindow::getFileName2()
{
	return _pixWidget2->getFileNmae().toStdString();
}


void OpenCVWindow::onDisplayResult(QImage &a)
{
	_pixWidget2->setImage(a);
}
void OpenCVWindow::onDisplayResult(cv::Mat &image111)
{
	QPixmap a = QPixmap::fromImage( myCV.Mat2QImage(image111));
	_pixWidget2->setImage(a);
}

void OpenCVWindow::onDisplayResult(Mat &image, QLabel *lb)
{
	QPixmap a = QPixmap::fromImage( myCV.Mat2QImage(image));
	lb->setPixmap(a.scaled(64,64));
}

void OpenCVWindow::onDetect(Mat &image)
{
	// Read reference image


		// Define ROI
		//cv::Mat imageROI= image(cv::Rect(110,260,35,40));
		//cv::rectangle(image, cv::Rect(110,260,35,40), cv::Scalar(0,0,255));

		// Select ROI
		Rect2d r = selectROI(image);
		cv::waitKey(0);
		cv::destroyAllWindows();

		cv::rectangle(image, r, cv::Scalar(0,0,255));
		auto mm = myCV.Mat2QImage(image) ;
		_pixWidget1->setImage( mm  );

		Mat imageROI = image(r);


		// Display image
		//cv::namedWindow("Image 1");
		//cv::imshow("Image 1",image);

		// Get the Hue histogram
		int minSat=65;
		ColorHistogram hc;
		cv::MatND colorhist= hc.getHueHistogram(imageROI, minSat);

		ObjectFinder finder;
		finder.setHistogram(colorhist);
		finder.setThreshold(0.2f);

		// Second image
		image= cv::imread(getFileName2());

		// Display image
		//cv::namedWindow("Image 2");
		//cv::imshow("Image 2",image);

		// Convert to HSV space
		cv::Mat hsv;
		cv::cvtColor(image, hsv, CV_BGR2HSV);

		// Split the image
		vector<cv::Mat> v;
		cv::split(hsv,v);

		// Eliminate pixels with low saturation
		cv::threshold(v[1],v[1],minSat,255,cv::THRESH_BINARY);
		//cv::namedWindow("Saturation");
		//cv::imshow("Saturation",v[1]);
		auto pp1 = QPixmap::fromImage( myCV.Mat2QImage1(v[1]));
		ui->pp1->setPixmap( pp1.scaled(60,60));

		// Get back-projection of hue histogram
		int ch[1]={0};
		cv::Mat result= finder.find(hsv,0.0f,180.0f,ch,1);

		//cv::namedWindow("Result Hue");
		//cv::imshow("Result Hue",result);

		auto pp2 = QPixmap::fromImage( myCV.Mat2QImage1(result));
		ui->pp2->setPixmap( pp2.scaled(60,60));

		// Eliminate low stauration pixels
		cv::bitwise_and(result,v[1],result);
		//cv::namedWindow("Result Hue and raw");
		//cv::imshow("Result Hue and raw",result);
		auto pp3 = QPixmap::fromImage( myCV.Mat2QImage1(result));
		ui->pp3->setPixmap( pp3.scaled(60,60));

		cv::Rect rect( r );
		cv::rectangle(image, r, cv::Scalar(0,0,255));

		cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER,10,0.01);
		cout << "meanshift= " << cv::meanShift(result,rect,criteria) << endl;

		cv::rectangle(image, r, cv::Scalar(0,255,0));

		// Display image
		//cv::namedWindow("Image 2 result");
		//cv::imshow("Image 2 result",image);
		auto pp4 = QPixmap::fromImage( myCV.Mat2QImage(image));
		_pixWidget2->setImage(pp4);
}

void OpenCVWindow::onHistogram1d(Mat &image)
{
	Mat hist = myCV.getHistogram(image);

	Mat result = myCV.getHistogramImage(hist);

	//cv::namedWindow("Histogram");
	//cv::imshow("Histogram",	 result);


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

	//Display the QImage in the Label
	QPixmap img_pix = QPixmap::fromImage(img_qt.rgbSwapped()); //BGR to RGB
   _pixWidget2->setImage(img_pix);
}

void OpenCVWindow::onMorpho(Mat &image)
{
	// Create the morphological features instance
	   MorphoFeatures morpho;
	   morpho.setThreshold(40);
	   // Get the edges
	   cv::Mat edges;
	   edges= morpho.getEdges(image);
	   onDisplayResult(edges);
}



void OpenCVWindow::onDetectAndDisplay( Mat frame )
{
	auto ff = myCV.face_detect(frame);
	onDisplayResult(ff);
}



int OpenCVWindow::onCapture()
{

	VideoCapture capture;
	Mat frame;
	  //-- 2. Read the video stream
	  capture.open( -1 );
	  if ( ! capture.isOpened() )
	  {
		  printf("--(!)Error opening video capture\n"); return -1;
	  }

	  while (  capture.read(frame) )
	  {
		  if( frame.empty() )
		  {
			  printf(" --(!) No captured frame -- Break!");
			  break;
		  }

		  //-- 3. Apply the classifier to the frame
		  onDetectAndDisplay( frame );
	  }
	  return 0;

   return 0;
}


void OpenCVWindow::onSharp(Mat &img)
{
	auto dst =  myCV.sharp_detect(img);
	onDisplayResult(dst);
}


void OpenCVWindow::slotMainSplitterMoved()
{
	if(mainSplitter->sizes()[1] != 0 )
		_sec_size = mainSplitter->sizes()[1];
}
void OpenCVWindow::handleDoubleClicked()
{
	auto szs = mainSplitter->sizes();
	if(szs[1] == 0)
		mainSplitter->setSizes(QList<int>() << width()- _sec_size << _sec_size);
	else
		mainSplitter->setSizes(QList<int>() << width() << 0);
}

