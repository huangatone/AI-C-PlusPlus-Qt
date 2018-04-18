#ifndef CVPICTUREVIEW_H
#define CVPICTUREVIEW_H

#include <QWidget>

#include <QTreeWidgetItem>
#include <QLabel>
#include <QFile>
#include <QDir>
#include "pictureview.h"
#include "tourist.h"

namespace Ui {
class CVPictureView;
}

class CVPictureView : public QWidget
{
	Q_OBJECT

public:
	explicit CVPictureView(QWidget *parent = 0);
	~CVPictureView();

private slots:
	void on_btnZoomIn_clicked();
	void on_btnZoomOut_clicked();
	void on_btnPre_clicked();
	void on_btnNext_clicked();

	void on_treeView_clicked(const QModelIndex &index);

	void on_btnWork_clicked();

private:
	Ui::CVPictureView *ui;

	QWidget* pa;
	QLabel* lb;
	QPixmap _img;
	QDir _dir;
	QString _file;
	QFileInfoList _list;
	int _current_index;

	PictureView* _pictureView;
	Tourist tourist;



	void setImage(const QString &file);
};

#endif // CVPICTUREVIEW_H
