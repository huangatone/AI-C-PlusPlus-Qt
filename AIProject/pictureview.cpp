#include "pictureview.h"
#include "ui_pictureview.h"

#include <QDebug>

PictureView::PictureView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PictureView)
{
	ui->setupUi(this);
	lb = new QLabel(this);
	ui->scrollArea->setWidget(lb);
	ui->scrollArea->setWidgetResizable(false);
}

PictureView::~PictureView()
{
	delete ui;
}



void PictureView::setImage(const QString &file)
{
	lb->setScaledContents(false);
	_img =QPixmap(file);
	lb->resize( _img.width(), _img.height());
	QFile fi(file);
	ui->label->setText( fi.fileName());
   lb->setPixmap( _img);

}
QString PictureView::dir() const
{
	return "" ;
}

void PictureView::setDir(const QString &dir)
{

}


void PictureView::on_btnPVZoomIn_clicked()
{
	const QPixmap *pix = lb->pixmap();
	lb->setScaledContents(true);
	lb->resize( lb->width() * 0.9, lb->height()* 0.9);
}

void PictureView::on_btnPVZoomOut_clicked()
{
	const QPixmap *pix = lb->pixmap();
	lb->setScaledContents(true);
	lb->resize( lb->width() *1.1, lb->height()*1.1);
}

void PictureView::on_btnPVZoom_clicked()
{
	lb->setScaledContents(true);
	lb->resize( ui->scrollArea->size() );
}

void PictureView::on_btnNext_clicked()
{

}

QString PictureView::file() const
{
	return _file;
}

void PictureView::setFile(const QString &file)
{
	QFile fi(file);
	if(fi.exists() == false)
		return;
	setImage(file);
	_file = file;


}

void PictureView::on_horizontalSlider_sliderMoved(int position)
{
	if(position == 0)
	{
		on_btnPVZoom_clicked();
		return;
	}
	float rat = position / 10.0f;
	lb->setScaledContents(true);
	if(rat >0)
		rat = 1.1f * rat;
	else
		rat = 0.9f * rat;
	lb->resize( lb->width() *rat, lb->height()*rat);
}



