#include "cvdialog.h"
#include "ui_cvdialog.h"
#include <QFileDialog>
#include "cvfunction.h"


CvDialog::CvDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CvDialog)
{
	ui->setupUi(this);
	_pic_label1= new QLabel(ui->scrollArea);
	ui->scrollArea->setWidget(_pic_label1);

	_pic_label2 = new QLabel(ui->scrollArea_2);
	ui->scrollArea_2->setWidget(_pic_label2);
}

CvDialog::~CvDialog()
{
	delete ui;
}

void CvDialog::on_toolButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName();
	QPixmap a = QPixmap(file_name);
	ui->label->setText( file_name);
	_pic_label1->resize( a.size());
	_pic_label1->setPixmap(a);
}

void CvDialog::on_toolButton_2_clicked()
{
	if(ui->lineEdit->text()!= "")
		_pic_label2->pixmap()->save(ui->lineEdit->text());
	else
		_pic_label2->pixmap()->save("/rong/tmp/1.png");
}

void CvDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	if( button == ((QAbstractButton*) ui->buttonBox->button(QDialogButtonBox::Apply)) )
	{
		CVFunction cd;
		Mat srcImage = imread(ui->label->text().toStdString());

		QPixmap a = QPixmap::fromImage( callbackFunc(srcImage));

		_pic_label2->resize( a.size());
		_pic_label2->setPixmap(a);

	}
}
