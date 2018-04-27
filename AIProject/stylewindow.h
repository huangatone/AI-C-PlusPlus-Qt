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

#include "pictureview.h"

namespace Ui {
class StyleWindow;
}

class StyleWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit StyleWindow(QWidget *parent = 0);
	~StyleWindow();

	void initWidgets(QString str);
	void addSubWindow(QWidget* w);
protected slots:
	void slot_icon_buttun_clicked();
private slots:
	void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
	void on_btnDo_clicked();
	void on_toolButton_5_clicked();

	void slotSetFile();
	void slotPreview();

	void on_functionList_clicked(const QModelIndex &index);

private:
	Ui::StyleWindow *ui;

	void process(QString cmd);

	void displayResult(Mat &m,QString title = "NoTitle");
	bool eventFilter(QObject *watched, QEvent *event);

	PictureView*_pictureView;


};

#endif // STYLEWINDOW_H
