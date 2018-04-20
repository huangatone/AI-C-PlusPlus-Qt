#include "rgbeditor.h"
#include "ui_rgbeditor.h"
#include <QIntValidator>
#include <QColorDialog>
#include <QDebug>
RGBEditor::RGBEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::RGBEditor)
{
	ui->setupUi(this);
	connect(ui->btn,SIGNAL(clicked()), this, SLOT(slotBtnClicked()));	
	ui->LERGB->setValidator( new QRegExpValidator( QRegExp("[#]{0,1}[0-9A-Fa-f]{0,6}"),this) );
}

RGBEditor::~RGBEditor()
{
	delete ui;
}

QColor RGBEditor::color()
{
	return _clr;
}

void RGBEditor::setColor(QColor clr,QColor default_clr)
{
	_clr = clr;
	_default = default_clr;
	ui->LERGB->disconnect();
	QPixmap backPix = QPixmap(16,16);

	if(clr.isValid() )
	{
		ui->LERGB->setText( clr.name().toUpper());
		backPix.fill(clr);
	}
	else
	{
		ui->LERGB->setText( "");
		backPix.fill(default_clr);
	}

	ui->btn->setIcon(QIcon(backPix));
	connect(ui->LERGB, SIGNAL(editingFinished()), this, SLOT(slotTextFinished() ));
	connect(ui->LERGB, SIGNAL(textEdited(const QString &)), this, SLOT(slotTextChanged() ));

}

void RGBEditor::setEditAble(bool bEdit)
{
	ui->LERGB->setReadOnly(!bEdit);
	ui->btn->setEnabled(bEdit);
}

bool RGBEditor::editAble()
{
	return ui->btn->isEnabled();
}

void RGBEditor::slotTextChanged()
{
	QString c = ui->LERGB->text();		
	//ui->LERGB->setText(c.toUpper());
	if(c.length() > 0 )
	{
		if (c.startsWith("#") && c.length() != 7)
			return;
		else if (c.startsWith("#")== false  && c.length() != 6)
			return;
	}
	if(c.startsWith("#")== false && c.length() == 6 )
		c  = "#" + c;
	ui->LERGB->setText(c.toUpper());
	QPixmap backPix = QPixmap(16,16);
	_clr = QColor (c);

	if(c.length() == 0)
		backPix.fill(_default);
	else
		backPix.fill(_clr);

	ui->btn->setIcon(QIcon(backPix));
	emit colorChanged(color());
}
void RGBEditor::slotTextFinished()
{
	QString c = ui->LERGB->text();
	if(_clr.isValid() )
	{
		ui->LERGB->setText( _clr.name().toUpper() );
	}
	else
		ui->LERGB->setText( "");
}

void RGBEditor::slotBtnClicked()
{
	QColor clr =QColor (ui->LERGB->text());
	clr = QColorDialog::getColor(clr, this);
	if(clr.isValid() == false)
		return;
	_clr = clr;
	setColor(_clr,_default);
	emit colorChanged(_clr);
}
