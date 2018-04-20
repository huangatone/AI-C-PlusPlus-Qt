#ifndef DEALVIEW_H
#define DEALVIEW_H

#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QListWidgetItem>
#include "pictureview.h"

#include <opencv/cv.h>
#include <opencv/cvwimage.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <QImage>
using namespace std;
using namespace cv;

namespace Ui {
class DealView;
}

class DealView : public QWidget
{
	Q_OBJECT

public:
	explicit DealView(QWidget *parent = 0);
	~DealView();

private slots:
	void on_listWidget_itemClicked(QListWidgetItem *item);

	void on_btnAdd_clicked();

	void on_btnDel_clicked();

	void on_btnRun_clicked();

	void on_btnRefresh_clicked();

	void on_btnReset_clicked();

	void on_btnAddFromTree_clicked();

	void on_sliderA_sliderMoved(int position);
	void on_sliderB_sliderMoved(int position);
	void on_sliderC_sliderMoved(int position);
	void on_sliderD_sliderMoved(int position);

private:
	Ui::DealView *ui;

	QWidget* pa;
	PictureView* _pictureView;

	QString _cmd;

	// QObject interface

	void displayResult(Mat &m,QString title="");
public:
	bool eventFilter(QObject *watched, QEvent *event);
	QString cmd() const;
	void setCmd(const QString &cmd);
};

#endif // DEALVIEW_H
