#include "cvpictureview.h"
#include "ui_cvpictureview.h"
#include <QFileSystemModel>
#include <QDebug>



CVPictureView::CVPictureView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CVPictureView)
{
	ui->setupUi(this);

	QFileSystemModel* pM = new QFileSystemModel (this);
	QStringList filters;
		 filters << "*.png" << "*.jpg" << "*.jpeg";
	//pM->setNameFilters(filters);
		 pM->setRootPath("/rong");
	ui->treeView->setModel(pM);

}

CVPictureView::~CVPictureView()
{
	delete ui;
}


void CVPictureView::on_btnPre_clicked()
{
	qDebug() << "pre" <<_current_index << _list.count();
	if(_current_index<0)
		_current_index = _list.count() - 1;
	if(_current_index >= _list.count())
		_current_index = _list.count() - 1;

	if(_list.count() == 0)
		return;
	auto f = _list[_current_index].absoluteFilePath();
	ui->widget->setFile( f);
	_current_index--;
}

void CVPictureView::on_btnNext_clicked()
{
	qDebug() << "next" <<_current_index << _list.count();
	if(_current_index >= _list.count())
		_current_index = 0;
	if(_current_index<0)
		_current_index = 0;
	if(_list.count() == 0)
		return;
	auto f = _list[_current_index].absoluteFilePath();
	ui->widget->setFile( f);
	_current_index++;
}

void CVPictureView::setImage(const QString &file)
{	
	QFile fi(file);
	if(fi.exists() == false)
		return;

   ui->widget->setFile( file);


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

void CVPictureView::setViewFile(QString f)
{
	ui->widget->setFile( f);
}


QString CVPictureView::getViewFile()
{
	return ui->widget->file();
}
