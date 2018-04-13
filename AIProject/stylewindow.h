#ifndef STYLEWINDOW_H
#define STYLEWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>


#include <opencv/cv.h>
#include <opencv/cvwimage.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <QImage>
using namespace std;
using namespace cv;
#include <QLabel>

namespace Ui {
class StyleWindow;
}

class StyleWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit StyleWindow(QWidget *parent = 0);
	~StyleWindow();

	void initButtons(QString str);
	void initTree(QString str);

	void initButtons(QString str, int i);
	void initTree(QString str, int i);

protected slots:
	void on_icon_buttun_clicked();
private slots:
	void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

	void on_btnDo_clicked();

	void on_btnFile1_clicked();

	void on_btnFile2_clicked();

	void on_btnFile3_clicked();

	void on_btnFile4_clicked();

private:
	Ui::StyleWindow *ui;


	void setFileName(QLabel* p);
	void displayResult(Mat &m);



	//Image Stitching
	void onImage_Stitching( QString file_name1, QString file_name2);

	//Advanced Edge Detection
	void onAdvanced_Edge_Detection(QString file_name,QString dir_name);


	//Colour Segmentation
	void onColour_Segmentation(QString file_name);

	//Colour Transfer
	void onColour_Transfer(QStringList file_lt);

	//----------------
	void onContrast_Enhancement(QString file_name);

	//---------
	void onEdge_Detection(QString file_name);


	//Histogram Equalization
	void onHistogram_Equalization(QString f_name);

	//Harris Corner Detector--
	void onHarris_Corner_Detector(QString f_name);

	//Image Sharpening
	void onImage_Sharpening(QString f_name);

	//Mat Image_Smoothing(QStringList f_lt)
	void onImage_Smoothing(QStringList f_lt);


	//Mat Image_Stitching(QStringList f_lt)
	void onImage_Stitching(QStringList f_lt);


	//Point Operations on Digital Images
	void onPoint_Operations_on_Digital_Images(QString f_n);

	//seam carving;
	void onseam_carving(QString f_n);
};

#endif // STYLEWINDOW_H