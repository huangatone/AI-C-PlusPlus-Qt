#include "dftoolbtn.h"
#include <QDebug>
#include <QPainter>


void paintLock(QPainter *P, const QRect &R, const QColor &C, int status)
{
	if (P && R.isValid()) {
		QRectF R0(R.x()+3, R.y(), R.width()-6, R.height()-2);
		QRectF R1(R.x()+1, R.y()+6, R.width()-2, R.height()-6);
		QPainterPath P0, P1;
		P0.addRoundedRect(R0, 4.0, 4.0);
		P1.addRoundedRect(R1, 1.0, 1.0);
		if (status == 0) {
			P->setPen(QPen(QColor(148, 148, 148), 1.1));
			P0 = P0.subtracted(P1);
			P->drawPath(P0);
			P->drawPath(P1);
		} else {
			P->setPen(QPen(C, 1.1));
			P->drawPath(P0);
			P->fillPath(P1, C);
		}
	}
}
void paintGrid(QPainter *P, const QRect &R, const QColor &C, int status)
{
	QRectF R0(R.x(), R.y(), R.width()+8, R.height());
	QRectF r0(R.x()+4,  R.y()+4,  5, 4);
	QRectF r1(R.x()+4,  R.y()+10, 5, 4);
	QRectF r2(R.x()+13, R.y()+4,  5, 4);
	QRectF r3(R.x()+13, R.y()+10, 5, 4);
	QPainterPath PP;

	P->setRenderHint(QPainter::Antialiasing);
	P->fillRect(R, Qt::transparent);

	P->drawRect(r0);
	P->drawRect(r1);
	P->drawRect(r2);
	P->drawRect(r3);
}

void Draw1()
{
	qDebug() << __FUNCTION__;
}
void Draw2()
{
	qDebug() << __FUNCTION__;
}
void Draw3()
{
	qDebug() << __FUNCTION__;
}


DFToolBtn::DFToolBtn(QWidget* p ):QToolButton (p)
{
	set(&paintLock);
setAutoFillBackground(true);
setMouseTracking(true);
}


void DFToolBtn::focusInEvent(QFocusEvent *event)
{
	qDebug() << __FUNCTION__;
	QPalette pal;
	pal.setBrush(QPalette::Window, QColor(104, 128, 164));
	pal.setBrush(QPalette::WindowText, QColor(255, 255, 255));
	setPalette(pal);
	update();
}

void DFToolBtn::focusOutEvent(QFocusEvent *event)
{
	qDebug() << __FUNCTION__;
	QPalette pal;
	pal.setBrush(QPalette::Window, QColor(104, 128, 0));
	pal.setBrush(QPalette::WindowText, QColor(255, 0, 255));
	setPalette(pal);
	update();
}

void DFToolBtn::paintEvent(QPaintEvent *event)
{
	QToolButton::paintEvent(event);
	QPainter p(this);
	drawFun( &p,rect(),QColor(),2);
}


void DFToolBtn::enterEvent(QEvent *event)
{
	qDebug() << __FUNCTION__;
	set(&paintGrid);
//	QPalette pal = palette();
//	pal.setBrush(QPalette::Window, QColor(0, 0, 164));
//	pal.setBrush(QPalette::WindowText, QColor(255, 255, 255));
//	setPalette(pal);
	setStyleSheet("background-color: rgb(44, 74, 82); color: rgb(255, 255, 255);");
	//setText("E");
	update();

}

void DFToolBtn::leaveEvent(QEvent *event)
{
	qDebug() << __FUNCTION__;
	set(&paintGrid);

	//setText("L");
	setStyleSheet("background-color: rgb(104, 128, 164); color: rgb(255, 255, 255);");

	update();
}


void DFToolBtn::checkStateSet()
{
	//setText( isChecked()? "C":"U");
	setStyleSheet("background-color: rgb(104, 188, 188); color: rgb(255, 255, 255);");

	update();
	set(&paintLock);
	qDebug() << __FUNCTION__;

}
