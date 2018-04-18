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
	_current_index = 0;

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
	return _dir.absolutePath() ;
}

void PictureView::setDir(const QString &dir)
{
	_dir = QDir(dir);
	QStringList filters;
		 filters << "*.png" << "*.jpg" << "*.jpeg";
	//	 dir.setNameFilters(filters);
	_list = _dir.entryInfoList(filters,QDir::Files,QDir::Name);
	_current_index =0;
}


void PictureView::on_btnZoomIn_clicked()
{
	const QPixmap *pix = lb->pixmap();
	lb->setScaledContents(true);
	lb->resize( lb->width() * 0.9, lb->height()* 0.9);
}

void PictureView::on_btnZoomOut_clicked()
{
	const QPixmap *pix = lb->pixmap();
	lb->setScaledContents(true);
	lb->resize( lb->width() *1.1, lb->height()*1.1);
}

void PictureView::on_btnPre_clicked()
{
	if(_current_index<0)
		_current_index = _list.count() - 1;
	if(_list.count() == 0)
		return;
	auto f = _list[_current_index].absoluteFilePath();
	setImage(f);
	_current_index--;
}

void PictureView::on_btnNext_clicked()
{
	if(_current_index == _list.count())
		_current_index = 0;
	if(_list.count() == 0)
		return;
	auto f = _list[_current_index].absoluteFilePath();
	setImage(f);
	_current_index++;
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

	QFileInfo pix_file(file);
	//qDebug() << pix_file.absolutePath() ;

	_dir = QDir (pix_file.absolutePath());

	_file = file;


	QStringList filters;
		 filters << "*.png" << "*.jpg" << "*.jpeg";
	//	 dir.setNameFilters(filters);
	_list = _dir.entryInfoList(filters,QDir::Files,QDir::Name);
	_current_index = 0;
	foreach (auto info, _list) {
		if( info.fileName() == fi.fileName())
			break;
		_current_index++;
	}


}
