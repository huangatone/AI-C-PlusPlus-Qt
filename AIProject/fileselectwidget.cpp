#include "fileselectwidget.h"
#include "ui_fileselectwidget.h"

#include <QFileDialog>

FileSelectWidget::FileSelectWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FileSelectWidget)
{
	ui->setupUi(this);
}

FileSelectWidget::~FileSelectWidget()
{
	delete ui;
}

void FileSelectWidget::on_toolButton_clicked()
{
	auto fi = QFileDialog::getOpenFileName(this,"Select Image","","Images (*.png *.jpeg *.jpg)");
	if(fi == "")
		return;
	ui->lineEdit->setText(fi);
}

QString FileSelectWidget::label_text() const
{
	return _label_text;
}

void FileSelectWidget::setLabel_text(const QString &label_text)
{
	_label_text = label_text;
	ui->label->setText(label_text);
}

QString FileSelectWidget::file_path() const
{
	return ui->lineEdit->text();
}

void FileSelectWidget::setFile_path(const QString &file_path)
{
	_file_path = file_path;
	ui->lineEdit->setText(file_path);
}

void FileSelectWidget::on_btnPreview_clicked()
{
	emit preview();
}

void FileSelectWidget::on_btnFromWidget_clicked()
{
	emit useCurrentPic();
}
