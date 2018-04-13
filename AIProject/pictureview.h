#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QDir>

namespace Ui {
class PictureView;
}

class PictureView : public QWidget
{
	Q_OBJECT

public:
	explicit PictureView(QWidget *parent = 0);
	~PictureView();

	QString dir() const;
	void setDir(const QString &dir);

	QString file() const;
	void setFile(const QString &file);

private slots:
	void on_btnZoomIn_clicked();

	void on_btnZoomOut_clicked();

	void on_btnPre_clicked();

	void on_btnNext_clicked();

private:

	void setImage(const QString &file);
	Ui::PictureView *ui;

	QLabel* lb;
	QPixmap _img;

	QDir _dir;
	QString _file;
	QFileInfoList _list;
	int _current_index;
};

#endif // PICTUREVIEW_H
