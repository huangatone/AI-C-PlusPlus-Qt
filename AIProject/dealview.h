#ifndef DEALVIEW_H
#define DEALVIEW_H

#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QListWidgetItem>
#include "pictureview.h"

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

	void on_horizontalSlider_sliderMoved(int position);

	void on_horizontalSlider_2_sliderMoved(int position);

	void on_btnAdd_clicked();

	void on_btnDel_clicked();

	void on_btnRun_clicked();

	void on_btnRefresh_clicked();

private:
	Ui::DealView *ui;
	QLabel* lb;
	QWidget* pa;
	PictureView* _pictureView;

	// QObject interface
public:
	bool eventFilter(QObject *watched, QEvent *event);
};

#endif // DEALVIEW_H
