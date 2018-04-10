#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "ConditionalMacros.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	cv::Mat image = imread("/Users/eternity/Documents/study/studyResource /cv/bovw/test image/books/book15.jpg");//("/Users/eternity/Documents/study/Identification of Spine(new)/query/book15.jpg");





}

MainWindow::~MainWindow()
{
	delete ui;
}
