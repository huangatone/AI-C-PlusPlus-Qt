
#include <QApplication>
#include <QDebug>

#include "stylewindow.h"
#include "cvfunction.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// QApplication::setStyle(QStyleFactory::create("macintosh"));

	StyleWindow w;

	w.show();

	CVFunction cd;

	//Mat out = Colour_Segmentation("/rong/tmp/hand.png");
	//cd.Mat2QImage1(out).save("/rong/tmp/hand1.png");

	//Advanced_Edge_Detection("/rong/tmp/tulips.png", "/rong/tmp/");
	//Mat out = Image_Stitching("/rong/tmp/Ryerson-left.jpg","/rong/tmp/Ryerson-right.jpg");
	//cd.Mat2QImage(out).save("/rong/tmp/edge1.png");
	/*
	QStringList fl;
	fl <<"/rong/tmp/paint.jpg" << "/rong/tmp/clock_tower.jpg";
	auto lt =  Colour_Transfer(fl);
	int index = 0;
	foreach (auto m, lt)
	{
		cd.Mat2QImage(m).save( QString("/rong/tmp/color_transfer%1.png").arg(index) );
		qDebug() << QString("/rong/tmp/color_transfer%1").arg(index);
		//cd.Mat2QImage1(m).save( QString("/rong/tmp/color_transfer_1%1.png").arg(index) );
		index++;
	}*/

	//auto out = Contrast_Enhancement("/rong/tmp/Haze6.jpg");
	//cd.Mat2QImage(out).save("/rong/tmp/Enhancement.png");

	Mat out;
	//out = Edge_Detection("/rong/tmp/tulips.png");
	//cd.Mat2QImage1(out).save("/rong/tmp/tulips_edge.png");

	//out = Harris_Corner_Detector("/rong/tmp/corner_detection_test.jpg");
	//cd.Mat2QImage1(out).save("/rong/tmp/Harris_Corner_Detector1.png");

	//out = Histogram_Equalization("/rong/tmp/Haze6.jpg");
	//cd.Mat2QImage(out).save("/rong/tmp/Haze6_Histogram_Equalization.png");

	//Image Sharpening
	//out =  Image_Sharpening("/rong/tmp/Haze6.jpg");
	//cd.Mat2QImage(out).save("/rong/tmp/Haze6_Image_Sharpening.png");
/*	QStringList fl;
	fl << "/rong/tmp/lena_gaussian_noise.png" << "/rong/tmp/lena_impulse_noise.jpg";
	Image_Smoothing(fl);

	fl.clear();
	fl << "/rong/tmp/Ryerson-left.jpg";
	fl << "/rong/tmp/Ryerson-right.jpg";
	out = Image_Stitching(fl);
	cd.Mat2QImage(out).save("/rong/tmp/Image_Stitching.png");
	//Point Operations on Digital Images
	out = 	Point_Operations_on_Digital_Images("/rong/tmp/rgb_squares.png");
	cd.Mat2QImage(out).save("/rong/tmp/Point_Operations_on_Digital_Images.png");


	out = seam_carving("/rong/tmp/ryerson.jpg");
	cd.Mat2QImage(out).save("/rong/tmp/seam_carving.png");
*/
	return a.exec();
}
