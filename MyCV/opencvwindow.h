#ifndef OPENCVWINDOW_H
#define OPENCVWINDOW_H

#include <QMainWindow>

namespace Ui {
class OpenCVWindow;
}


#include <QSplitter>
#include <QLabel>

#include "cvfunction.h"
#include <opencv2/tracking.hpp>

class DFPixmapWdiget;
using namespace std;


class  OpenCVWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit OpenCVWindow(QWidget *parent = 0);
	~OpenCVWindow();

private slots:
	void on_btnProcess_clicked();
	void slotMainSplitterMoved();
	void handleDoubleClicked();

private:
	Ui::OpenCVWindow *ui;

	QSplitter* mainSplitter;
	DFPixmapWdiget* _pixWidget1;
	DFPixmapWdiget* _pixWidget2;

	CVFunction myCV;


	int histSize[1];
	float hranges[2];
	const float* ranges[1];
	int channels[1];
	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);

protected:
	void onSalt();
	void onReduce();
	void onChannel(Mat &img);
	void onLogo(Mat &img);
	void onFlip(Mat &img);
	void onAdd(Mat &img);
	void on2Logo(Mat &img);
	void onColor(uint r, uint g, uint b);
	void onSharp(Mat &img);

	string getFileName1();
	string getFileName2();

	void onDisplayResult(QImage &a);
	void onDisplayResult(Mat &image);
	void onDisplayResult(Mat &image,QLabel* lb);
	void onDetect(Mat &img);
	void onHistogram1d(Mat &img);
	void onMorpho(Mat &img);
	void onDetectAndDisplay( Mat frame );
	int onCapture();
	void GetTextFromPicture(Mat frame);

	int _sec_size;


	// QObject interface
public:
	bool eventFilter(QObject *watched, QEvent *event);
};

#endif // OPENCVWINDOW_H
