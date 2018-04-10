#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <opencv/cv.h>
#include <opencv/cvwimage.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <QImage>
using namespace std;
using namespace cv;

#include "cvfunction.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();


	void find();

private slots:
	void on_toolButton_clicked();

	void on_toolButton_2_clicked();

	void on_toolButton_3_clicked();

	void on_toolButton_4_clicked();

private:
	Ui::MainWindow *ui;
	CVFunction cd;

	static QImage do_findSharp(Mat src);
	static int _fun_index;

};

#endif // MAINWINDOW_H
