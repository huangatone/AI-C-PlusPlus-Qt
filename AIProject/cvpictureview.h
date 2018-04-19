#ifndef CVPICTUREVIEW_H
#define CVPICTUREVIEW_H

#include <QWidget>

#include <QTreeWidgetItem>
#include <QLabel>
#include <QFile>
#include <QDir>
#include "pictureview.h"


namespace Ui {
class CVPictureView;
}

class CVPictureView : public QWidget
{
	Q_OBJECT

public:
	explicit CVPictureView(QWidget *parent = 0);
	~CVPictureView();

	void setViewFile(QString f);
	QString getViewFile();

private slots:
	void on_btnPre_clicked();
	void on_btnNext_clicked();
	void on_treeView_clicked(const QModelIndex &index);
private:
	Ui::CVPictureView *ui;
	QPixmap _img;
	QDir _dir;
	QString _file;
	QFileInfoList _list;
	int _current_index;
	void setImage(const QString &file);
};

#endif // CVPICTUREVIEW_H
