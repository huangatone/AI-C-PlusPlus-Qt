#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <QLabel>
#include <QDebug>
#include <QSpacerItem>
#include <QListWidget>


CustomInputDialog::CustomInputDialog(QWidget *parent):
	QDialog(parent),
	ui(new Ui::InputDialog)
{
	ui->setupUi(this);
	_layout = new QGridLayout(this);
	setLayout(_layout);
	setWindowTitle( tr("Please input datas"));
	index = 0;

}

CustomInputDialog::~CustomInputDialog()
{

}

void CustomInputDialog::addWidget(QString lbs, QWidget *w)
{
	QLabel * lbl = new QLabel(lbs, this);
	lbl->setAlignment(Qt::AlignTop);
	_layout->addWidget(lbl, index,0);
	_layout->addWidget(w, index,1);
	index++;
}

void CustomInputDialog::addWidget(QWidget* l, QWidget *r)
{
	_layout->addWidget(l, index,0);
	_layout->addWidget(r, index,1);
	index++;
}

void CustomInputDialog::endAdd()
{
	_layout->addItem( new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Expanding) ,index,0);
	index++;
	_layout->addWidget( ui->buttonBox,index,0,1,2);
}

int CustomInputDialog::exec()
{
	endAdd();
	return  QDialog::exec();
}
