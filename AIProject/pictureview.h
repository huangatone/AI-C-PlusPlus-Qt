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


	void on_btnNext_clicked();

	void on_horizontalSlider_sliderMoved(int position);

	void on_btnPVZoomIn_clicked();

	void on_btnPVZoomOut_clicked();

	void on_btnPVZoom_clicked();

private:

	void setImage(const QString &file);
	Ui::PictureView *ui;

	QLabel* lb;
	QPixmap _img;


	QString _file;


	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
};

#endif // PICTUREVIEW_H
