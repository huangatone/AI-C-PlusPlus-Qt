#include "cvpictureview.h"
#include "ui_cvpictureview.h"
#include <QFileSystemModel>
#include <QDebug>
#include "stylewindow.h"


CVPictureView::CVPictureView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CVPictureView)
{
	ui->setupUi(this);
	pa = parent;
	lb = new QLabel(this);
	ui->scrollArea->setWidget(lb);
	ui->scrollArea->setWidgetResizable(false);
	QFileSystemModel* pM = new QFileSystemModel (this);
	QStringList filters;
		 filters << "*.png" << "*.jpg" << "*.jpeg";
	//pM->setNameFilters(filters);
		 pM->setRootPath("/rong");
	ui->treeView->setModel(pM);
	_pictureView = nullptr;
}

CVPictureView::~CVPictureView()
{
	delete ui;
}

void CVPictureView::on_btnZoomIn_clicked()
{
	const QPixmap *pix = lb->pixmap();
	lb->setScaledContents(true);
	lb->resize( lb->width() * 0.9, lb->height()* 0.9);
}

void CVPictureView::on_btnZoomOut_clicked()
{
	const QPixmap *pix = lb->pixmap();
	lb->setScaledContents(true);
	lb->resize( lb->width() *1.1, lb->height()*1.1);
}

void CVPictureView::on_btnPre_clicked()
{
	if(_current_index<0)
		_current_index = _list.count() - 1;
	if(_list.count() == 0)
		return;
	auto f = _list[_current_index].absoluteFilePath();
	setImage(f);
	_current_index--;
}

void CVPictureView::on_btnNext_clicked()
{
	if(_current_index == _list.count())
		_current_index = 0;
	if(_list.count() == 0)
		return;
	auto f = _list[_current_index].absoluteFilePath();
	setImage(f);
	_current_index++;
}

void CVPictureView::setImage(const QString &file)
{
	lb->setScaledContents(false);
	_img =QPixmap(file);
	lb->resize( _img.width(), _img.height());
	QFile fi(file);
	ui->label->setText( fi.fileName());
   lb->setPixmap( _img);

}

void CVPictureView::on_treeView_clicked(const QModelIndex &index)
{
	const QFileSystemModel* pM = dynamic_cast<const QFileSystemModel*>( index.model() );
	auto file = pM->fileName(index);
	qDebug() << file;
	if(file.endsWith( ".png", Qt::CaseInsensitive)  || file.endsWith( ".jpg", Qt::CaseInsensitive)
			|| file.endsWith( ".jpeg", Qt::CaseInsensitive) || file.endsWith( ".bmp", Qt::CaseInsensitive) )
	{
		auto info = pM->fileInfo(index);
		setImage(info.absoluteFilePath());
	}
}

void CVPictureView::on_btnWork_clicked()
{
	const QFileSystemModel* pM = dynamic_cast<const QFileSystemModel*>( ui->treeView->model()  );
	auto index = ui->treeView->currentIndex();
	auto file = pM->fileName(index);
	qDebug() << file;
	if(file.endsWith( ".png", Qt::CaseInsensitive)  || file.endsWith( ".jpg", Qt::CaseInsensitive)
			|| file.endsWith( ".jpeg", Qt::CaseInsensitive) || file.endsWith( ".bmp", Qt::CaseInsensitive) )
	{
		auto info = pM->fileInfo(index);
		//setImage(info.absoluteFilePath());
		if( _pictureView == nullptr)
		{
			StyleWindow* p = dynamic_cast<StyleWindow*>( pa );
			if(!p)
			{
				qDebug() << "No Parent";
				return;
			}
			_pictureView = new PictureView(p);
			//ui->mdiArea->addSubWindow(view);

			p->addSubWindow(_pictureView);
			_pictureView->show();
		}
		//_pictureView->setFile(info.absoluteFilePath());

		Mat m = tourist.onCandy(info.absoluteFilePath(), ui->horizontalSlider->value());
		imwrite("/rong/tmp/cv_tmp.png",m);
		_pictureView->setFile("/rong/tmp/cv_tmp.png");

	}
}
