#include "dfpixmapwdiget.h"
#include "ui_dfpixmapwdiget.h"
#include <QFileDialog>

DFPixmapWdiget::DFPixmapWdiget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DFPixmapWdiget)
{
	ui->setupUi(this);

	 ui->lbPix->setPixmap(QPixmap(QString::fromUtf8(":/images/road.jpg")));
}

DFPixmapWdiget::~DFPixmapWdiget()
{
	delete ui;
}

QString DFPixmapWdiget::getFileNmae()
{
	return ui->lbName->text();
}

void DFPixmapWdiget::setImage(QImage &img)
{
	QPixmap a = QPixmap::fromImage(img).scaled( ui->lbPix->width(), ui->lbPix->height() );

	ui->lbPix->setScaledContents(true);
	ui->lbPix->setPixmap(a);
}

void DFPixmapWdiget::setImage(QPixmap &img)
{
	QPixmap a = img.scaled( ui->lbPix->width(), ui->lbPix->height() );
	ui->lbPix->setScaledContents(true);
	ui->lbPix->setPixmap(a);
}

void DFPixmapWdiget::on_btnOpen_clicked()
{
	auto file_name = QFileDialog::getOpenFileName();
	QPixmap a = QPixmap(file_name).scaled( ui->lbPix->width(), ui->lbPix->height() );
	ui->lbName-> setText(file_name);
	ui->lbPix->setScaledContents(true);
	ui->lbPix->setPixmap(a);
}

void DFPixmapWdiget::on_toolButton_clicked()
{
	auto file_name = QFileDialog::getSaveFileName();
	ui->lbPix->pixmap()->save(file_name);
}
