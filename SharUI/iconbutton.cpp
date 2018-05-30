#include "iconbutton.h"
#include "ui_iconbutton.h"
#include <QMouseEvent>

IconButton::IconButton(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IconButton)
{
	ui->setupUi(this);
	setMouseTracking(true);
	ui->label->setMouseTracking(true);
	ui->label_2->setMouseTracking(true);

	ui->label->installEventFilter(this);
	ui->label_2->installEventFilter(this);
}

IconButton::~IconButton()
{
	delete ui;
}

void IconButton::SetIcon(QPixmap pix)
{
	ui->label->setPixmap(  pix.scaled(24,24));
}

void IconButton::SetText(QString text)
{
	ui->label_2->setText(text);
}

bool IconButton::eventFilter(QObject *watched, QEvent *event)
{
	if(watched == ui->label || watched == ui->label_2)
	{
		if(event->type() == QEvent::MouseButtonRelease)
		{
			QMouseEvent * e = dynamic_cast<QMouseEvent*>(event);
			if(e->button() != Qt::LeftButton)
				return false;
			emit clicked();
		}
	}
	return QWidget::eventFilter(watched,event);
}
QString IconButton::getText()
{
	return ui->label_2->text();
}
